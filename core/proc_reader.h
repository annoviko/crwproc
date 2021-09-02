/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once

#include <chrono>
#include <condition_variable>
#include <exception>
#include <functional>
#include <limits>
#include <memory>
#include <mutex>
#include <optional>
#include <ratio>
#include <thread>
#include <vector>

#include <windows.h>
#include <psapi.h>
#include <tlhelp32.h>

#include "filter.h"
#include "parallel.h"
#include "proc_handle.h"
#include "proc_pointer.h"
#include "proc_info.h"
#include "traits.h"


#undef max
#undef min


template <typename TypeFilter>
class proc_reader {
private:
    using memblock_container = std::vector<MEMORY_BASIC_INFORMATION>;

    struct proc_memblocks {
        memblock_container blocks;
        std::uint64_t total_size = 0;
    };

    struct proc_memory {
        bool is_valid = false;
        std::uint64_t addr_begin = std::numeric_limits<std::uint64_t>::max();
        std::uint64_t addr_end = 0;
        std::shared_ptr<std::uint8_t[]> memory;
    };

    using cache_memory = std::unordered_map<std::uint64_t, proc_memory>;

public:
    using progress_observer = std::function<void(std::size_t)>;

private:
    proc_info            m_proc_info;
    TypeFilter           m_filter;
    bool                 m_force_parallel = false;

    /* statistics */
    mutable std::uint64_t   m_bytes_to_read = 0;
    mutable std::uint64_t   m_bytes_read = 0;

    progress_observer               m_observer;
    mutable std::mutex              m_event_mutex;
    mutable std::condition_variable m_event;
    mutable bool                    m_stop_notifier = false;
    mutable std::thread             m_notifer;

private:
    static constexpr std::uint64_t READ_VALUE_SIZE = 16;

    static constexpr std::uint64_t TRIGGER_CPU_PARALLEL_BYTES = 10 * 1000 * 1000;     /* 10Mbytes */
    static constexpr std::uint64_t TRIGGER_CPU_PARALLEL_ITEMS = 10000;
    static constexpr std::uint64_t TRIGGER_CACHE_MEMORY_ITEMS = 10000;

    static constexpr std::uint64_t INVALID_PROC_SIZE = std::numeric_limits<std::uint64_t>::max();
    static constexpr std::uint64_t INVALID_INTEGER_VALUE = std::numeric_limits<std::uint64_t>::max();

    static constexpr std::size_t   NOTIFICATION_PERIOD_MS = 200;

public:
    proc_reader(const proc_info& p_info, const TypeFilter& p_filter) :
        m_proc_info(p_info),
        m_filter(p_filter)
    { }


    ~proc_reader() {
        stop_notifier();
    }

public:
    proc_pointer_sequence read_and_filter() const {
        proc_handle proc_handler(m_proc_info.pid(), proc_handle::access::read);

        proc_memblocks info_blocks = get_proc_memblocks(proc_handler);

        m_bytes_to_read = info_blocks.total_size;
        m_bytes_read = 0;
        run_notifier();

        proc_pointer_sequence result = read_and_filter_eval(proc_handler, { }, info_blocks);

        stop_notifier();
        return result;
    }


    proc_pointer_sequence read_and_filter(const proc_pointer_sequence& p_values) const {
        m_bytes_to_read = p_values.size() * m_filter.get_value_size();
        m_bytes_read = 0;
        run_notifier();

        proc_handle proc_handler(m_proc_info.pid(), proc_handle::access::read);
        proc_pointer_sequence result = read_and_filter_eval(proc_handler, p_values, { });

        stop_notifier();
        return result;
    }


    proc_pointer_sequence read(const proc_pointer_sequence& p_values) const {
        proc_handle proc_handler(m_proc_info.pid(), proc_handle::access::read);

        if (m_force_parallel || p_values.size() > TRIGGER_CPU_PARALLEL_ITEMS) {
            return read_parallel_cpu(proc_handler, p_values);
        }

        return read_sequentially(proc_handler, p_values);
    }


    void subscribe(const progress_observer& p_observer) {
        m_observer = p_observer;
    }


    void force_parallel_processing() {
        m_force_parallel = true;
    }

private:
    proc_pointer_sequence combine_partial_results(const std::vector<proc_pointer_sequence>& partial_results) const {
        std::uint64_t length = 0;
        for (const auto& result : partial_results) {
            length += result.size();
        }

        proc_pointer_sequence total_result;
        total_result.reserve(length + 1);
        for (const auto& result : partial_results) {
            total_result.insert(total_result.end(), result.cbegin(), result.cend());
        }

        return total_result;
    }


    proc_pointer_sequence read_sequentially(const proc_handle& p_handle, const proc_pointer_sequence& p_values) const {
        proc_pointer_sequence result;
        result.reserve(p_values.size());

        for (const auto& pointer : p_values) {
            const proc_pointer new_pointer = read_value(p_handle, pointer.get_address(), pointer.get_base_address(), pointer.get_value());
            result.push_back(new_pointer);
        }

        return result;
    }


    proc_pointer_sequence read_parallel_cpu(const proc_handle& p_handle, const proc_pointer_sequence& p_values) const {
        std::vector<proc_pointer_sequence> partial_results(crwproc::parallel::get_amount_threads());

        auto task = [this, &p_handle, &p_values, &partial_results](const std::size_t p_idx, const std::size_t p_tidx) {
            const proc_pointer new_pointer = read_value(p_handle, p_values[p_idx].get_address(), p_values[p_idx].get_base_address(), p_values[p_idx].get_value());
            partial_results[p_tidx].push_back(new_pointer);
        };

        crwproc::parallel::parallel_for_with_tidx(std::size_t(0), p_values.size(), task);

        return combine_partial_results(partial_results);
    }


    proc_pointer_sequence read_and_filter_eval(const proc_handle& p_proc_handler, const proc_pointer_sequence& p_values, const proc_memblocks& p_info_blocks) const {
        switch (m_filter.get_value_type()) {
        case value::type::integral:
            switch (m_filter.get_value_size()) {
            case 1:
                return read_and_filter_with_type<std::uint8_t>(p_proc_handler, p_values, p_info_blocks);

            case 2:
                return read_and_filter_with_type<std::uint16_t>(p_proc_handler, p_values, p_info_blocks);

            case 4:
                return read_and_filter_with_type<std::uint32_t>(p_proc_handler, p_values, p_info_blocks);

            case 8:
                return read_and_filter_with_type<std::uint64_t>(p_proc_handler, p_values, p_info_blocks);

            default:
                throw std::logic_error("Invalid integer size '" + std::to_string(m_filter.get_value_size()) + "' is used by the filter.");
            }

        case value::type::floating:
            return read_and_filter_with_type<float>(p_proc_handler, p_values, p_info_blocks);

        case value::type::doubling:
            return read_and_filter_with_type<double>(p_proc_handler, p_values, p_info_blocks);

        default:
            throw std::logic_error("Unkown value type '" + std::to_string(static_cast<int>(m_filter.get_value_type())) + "' is used for filtering.");
        }
    }


    std::uint64_t get_proc_size(const std::uint64_t p_pid) const {
        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, static_cast<DWORD>(p_pid));
        std::uint64_t proc_size = INVALID_PROC_SIZE;

        if (snapshot != INVALID_HANDLE_VALUE) {
            MODULEENTRY32 module_info = { 0 };
            module_info.dwSize = sizeof(MODULEENTRY32);

            if (Module32First(snapshot, &module_info)) {
                proc_size = module_info.modBaseSize;
            }

            CloseHandle(snapshot);
        }

        return proc_size;
    }


    proc_pointer read_value(const proc_handle& p_proc_handler, const std::uint64_t p_address, const std::uint64_t p_base_address, const value& p_value) const {
        std::uint8_t buffer[READ_VALUE_SIZE];
        std::memset(buffer, 0x00, READ_VALUE_SIZE);

        std::uint64_t bytes_to_read = p_value.get_size();
        std::uint64_t bytes_was_read = 0;

        if (!ReadProcessMemory(p_proc_handler(), (LPCVOID)p_address, buffer, bytes_to_read, (SIZE_T*)&bytes_was_read)) {
            return { p_address, proc_pointer::INVALID_ADDRESS, value{ } };
        }

        return { p_address, p_base_address, value(p_value.get_type(), p_value.get_size(), p_value.is_signed(), buffer) };
    }


    proc_memblocks get_proc_memblocks(const proc_handle& p_handle) const {
        MEMORY_BASIC_INFORMATION  memory_info;

        std::uint64_t current_address = 0;

        proc_memblocks result;

        while (VirtualQueryEx(p_handle(), (LPCVOID)current_address, &memory_info, sizeof(memory_info))) {
            if (memory_info.State == MEM_COMMIT) {
                result.total_size += memory_info.RegionSize;
                result.blocks.push_back(memory_info);
            }

            current_address += memory_info.RegionSize;
        }

        return result;
    }


    std::uint64_t get_progress() const {
        if (m_bytes_read != 0) {
            return m_bytes_read * 100 / m_bytes_to_read;
        }

        return 0;
    }


    void run_notifier() const {
        if (m_observer) {
            m_stop_notifier = false;
            m_notifer = std::thread(&proc_reader::notifier_thread, this);
        }
    }


    void stop_notifier() const {
        if (!m_notifer.joinable()) {
            return;
        }

        {
            std::unique_lock<std::mutex> guard(m_event_mutex);
            m_stop_notifier = true;
        }

        m_event.notify_one();
        m_notifer.join();
    }


    void notifier_thread() const {
        std::unique_lock<std::mutex> guard(m_event_mutex);

        while (!m_stop_notifier) {
            m_observer(get_progress());
            m_event.wait_for(guard, std::chrono::milliseconds(NOTIFICATION_PERIOD_MS), [this]() { return m_stop_notifier; });
        }

        m_observer(100);
    }


private:
    template <typename TypeValue>
    inline void read_and_filter_whole_process_iteration(const proc_handle& p_handle, const MEMORY_BASIC_INFORMATION& p_memory_info, proc_pointer_sequence& p_result) const {
        std::shared_ptr<std::uint8_t[]> buffer(new std::uint8_t[p_memory_info.RegionSize]);
        std::memset(buffer.get(), 0x00, p_memory_info.RegionSize);

        std::uint64_t bytes_was_read = 0;

        if (ReadProcessMemory(p_handle(), (LPCVOID)p_memory_info.BaseAddress, buffer.get(), p_memory_info.RegionSize, (SIZE_T*)&bytes_was_read)) {
            extract_values<TypeValue>(buffer.get(), bytes_was_read, (std::uint64_t)p_memory_info.BaseAddress, m_filter, p_result);
            m_bytes_read += bytes_was_read;
        }
    }


    template <typename TypeValue>
    proc_pointer_sequence read_and_filter_whole_process_sequentially(const proc_handle& p_handle, const proc_memblocks& p_info_blocks) const {
        proc_pointer_sequence result;

        if constexpr (crwproc::traits::is_any<TypeFilter, filter_more, filter_less>::value) {
            result.reserve(p_info_blocks.total_size);
        }

        const auto& info_blocks = p_info_blocks.blocks;

        for (const auto& memory_info : info_blocks) {
            read_and_filter_whole_process_iteration<TypeValue>(p_handle, memory_info, result);
        }

        return result;
    }


    template <typename TypeValue>
    proc_pointer_sequence read_and_filter_whole_process_parallel_cpu(const proc_handle& p_handle, const proc_memblocks& p_info_blocks) const {
        std::vector<proc_pointer_sequence> partial_results(crwproc::parallel::get_amount_threads());

        if constexpr (crwproc::traits::is_any<TypeFilter, filter_more, filter_less>::value) {
            for (auto& thread_result : partial_results) {
                thread_result.reserve(p_info_blocks.total_size / crwproc::parallel::get_amount_threads());
            }
        }

        const auto& info_blocks = p_info_blocks.blocks;

        auto task = [this, &p_handle, &info_blocks, &partial_results](const std::size_t p_idx, const std::size_t p_tidx) {
            const auto& memory_info = info_blocks[p_idx];
            read_and_filter_whole_process_iteration<TypeValue>(p_handle, memory_info, partial_results[p_tidx]);
        };

        crwproc::parallel::parallel_for_with_tidx(std::size_t(0), info_blocks.size(), task);

        return combine_partial_results(partial_results);
    }


    template <typename TypeValue>
    inline void read_and_filter_values_iteration(const proc_handle& p_handle, const proc_pointer& p_pointer, proc_pointer_sequence& p_result) const {
        TypeValue actual_value;

        if (!ReadProcessMemory(p_handle(), (LPCVOID)p_pointer.get_address(), (void *) &actual_value, sizeof(TypeValue), nullptr)) {
            return;
        }

        if constexpr (crwproc::traits::is_any <TypeFilter, filter_more, filter_less>::value) {
            if (m_filter.is_satisfying(actual_value, p_pointer.get_value().get<TypeValue>())) {
                p_result.emplace_back(p_pointer.get_address(), p_pointer.get_base_address(), value::create(actual_value));
            }
        }
        else {
            if (m_filter.is_satisfying(actual_value)) {
                p_result.emplace_back(p_pointer.get_address(), p_pointer.get_base_address(), value::create(actual_value));
            }
        }

        m_bytes_read += sizeof(TypeValue);
    }


    template <typename TypeValue>
    inline void read_and_filter_values_iteration(const proc_handle& p_handle, const proc_pointer& p_pointer, const cache_memory& p_cache, proc_pointer_sequence& p_result) const {
        auto iter = p_cache.find(p_pointer.get_base_address());
        if (iter == p_cache.end()) {
            return;
        }

        const proc_memory& block = iter->second;
        if (!block.is_valid) {
            return;
        }

        std::uint64_t offset = p_pointer.get_address() - block.addr_begin;
        std::uint8_t* base_address = (std::uint8_t*)block.memory.get();
        TypeValue actual_value = *((TypeValue *)(base_address + offset));

        if constexpr (crwproc::traits::is_any <TypeFilter, filter_more, filter_less>::value) {
            if (m_filter.is_satisfying(actual_value, p_pointer.get_value().get<TypeValue>())) {
                p_result.emplace_back(p_pointer.get_address(), p_pointer.get_base_address(), value::create(actual_value));
            }
        }
        else {
            if (m_filter.is_satisfying(actual_value)) {
                p_result.emplace_back(p_pointer.get_address(), p_pointer.get_base_address(), value::create(actual_value));
            }
        }

        m_bytes_read += sizeof(TypeValue);
    }


    template <typename TypeValue>
    cache_memory cache_regions(const proc_handle& p_handle, const proc_pointer_sequence& p_values) const {
        cache_memory memory_blocks;
        for (const auto& pointer : p_values) {
            proc_memory& memory = memory_blocks[pointer.get_base_address()];

            memory.addr_begin = std::min(memory.addr_begin, pointer.get_address());
            memory.addr_end = std::max(memory.addr_end, pointer.get_address() + sizeof(TypeValue));
        }

        for (auto& pair : memory_blocks) {
            proc_memory& memory_block = pair.second;

            const std::uint64_t block_size = memory_block.addr_end - memory_block.addr_begin;
            memory_block.memory.reset(new std::uint8_t[block_size]);
            memory_block.is_valid = ReadProcessMemory(p_handle(), (LPCVOID)memory_block.addr_begin, memory_block.memory.get(), block_size, nullptr);
            if (!memory_block.is_valid) {
                memory_block.memory.reset();
            }
        }

        return memory_blocks;
    }


    template <typename TypeValue>
    proc_pointer_sequence read_and_filter_values_sequentially(const proc_handle& p_handle, const proc_pointer_sequence& p_values) const {
        proc_pointer_sequence result;

        if (p_values.size() >= TRIGGER_CACHE_MEMORY_ITEMS) {
            const cache_memory memory_blocks = cache_regions<TypeValue>(p_handle, p_values);
            for (const auto& pointer : p_values) {
                read_and_filter_values_iteration<TypeValue>(p_handle, pointer, memory_blocks, result);
            }
        }
        else {
            for (const auto& pointer : p_values) {
                read_and_filter_values_iteration<TypeValue>(p_handle, pointer, result);
            }
        }

        return result;
    }


    template <typename TypeValue>
    proc_pointer_sequence read_and_filter_values_parallel_cpu(const proc_handle& p_handle, const proc_pointer_sequence& p_values) const {
        std::vector<proc_pointer_sequence> partial_results(crwproc::parallel::get_amount_threads());

        /* Optimization. System call to read each variable is very expensive operation and
           if there are a lot of variables then it is much better to load memory blocks. */
        cache_memory cache;
        if (p_values.size() >= TRIGGER_CACHE_MEMORY_ITEMS) {
            cache = cache_regions<TypeValue>(p_handle, p_values);

            crwproc::parallel::parallel_for_with_tidx(std::size_t(0), p_values.size(), 
                [this, &p_handle, &p_values, &cache, &partial_results](const std::size_t p_idx, const std::size_t p_tidx) {
                    read_and_filter_values_iteration<TypeValue>(p_handle, p_values[p_idx], cache, partial_results[p_tidx]);
                }
            );
        }
        else {
            crwproc::parallel::parallel_for_with_tidx(std::size_t(0), p_values.size(),
                [this, &p_handle, &p_values, &partial_results](const std::size_t p_idx, const std::size_t p_tidx) {
                    read_and_filter_values_iteration<TypeValue>(p_handle, p_values[p_idx], partial_results[p_tidx]);
                }
            );
        }

        return combine_partial_results(partial_results);
    }


    template <typename TypeValue>
    proc_pointer_sequence read_and_filter_with_type(const proc_handle& p_handle, const proc_pointer_sequence& p_values, const proc_memblocks& p_info_blocks) const {
        if (p_values.empty()) {
            if (m_force_parallel || (p_info_blocks.total_size >= TRIGGER_CPU_PARALLEL_BYTES)) {
                return read_and_filter_whole_process_parallel_cpu<TypeValue>(p_handle, p_info_blocks);
            }

            return read_and_filter_whole_process_sequentially<TypeValue>(p_handle, p_info_blocks);
        }

        if (m_force_parallel || (p_values.size() >= TRIGGER_CPU_PARALLEL_ITEMS)) {
            return read_and_filter_values_parallel_cpu<TypeValue>(p_handle, p_values);
        }

        return read_and_filter_values_sequentially<TypeValue>(p_handle, p_values);
    }


    template <typename TypeValue>
    inline void extract_values(const std::uint8_t* p_buffer, const std::uint64_t p_length, const std::uint64_t p_base_address, const TypeFilter& p_filter, proc_pointer_sequence& p_result) const {
        for (std::uint64_t offset = 0; offset < p_length; ++offset) {
            const TypeValue actual_value = *((TypeValue*)(p_buffer + offset));

            if constexpr (crwproc::traits::is_any<TypeFilter, filter_more, filter_less>::value) {
                p_result.emplace_back(p_base_address + offset, p_base_address, value::create(actual_value));
            }
            else {
                if (p_filter.is_satisfying(actual_value)) {
                    p_result.emplace_back(p_base_address + offset, p_base_address, value::create(actual_value));
                }
            }
        }
    }
};