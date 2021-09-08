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
#include "memory_block.h"
#include "parallel.h"
#include "proc_handle.h"
#include "proc_pointer.h"
#include "proc_info.h"
#include "search_result.h"
#include "traits.h"


#undef max
#undef min


template <typename TypeFilter = filter_none>
class proc_reader {
private:
    using memblock_info_container = std::vector<MEMORY_BASIC_INFORMATION>;

    struct memblocks_info {
        memblock_info_container blocks;
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
    proc_handle          m_proc_handle;

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
    proc_reader(const proc_info& p_info) :
        m_proc_info(p_info),
        m_filter(filter_none{ }),
        m_proc_handle(m_proc_info.pid(), proc_handle::access::read)
    { }

    proc_reader(const proc_info& p_info, const TypeFilter& p_filter) :
        m_proc_info(p_info),
        m_filter(p_filter),
        m_proc_handle(m_proc_info.pid(), proc_handle::access::read)
    { }


    ~proc_reader() {
        stop_notifier();
    }

public:
    search_result read_and_filter() const {
        memblocks_info info_blocks = get_proc_memblocks();

        m_bytes_to_read = info_blocks.total_size;
        m_bytes_read = 0;
        run_notifier();

        search_result result = read_and_filter_eval({ }, info_blocks);

        stop_notifier();
        return result;
    }


    search_result read_and_filter(const search_result& p_values) const {
        m_bytes_to_read = p_values.get_size();
        m_bytes_read = 0;
        run_notifier();

        search_result result = read_and_filter_eval(p_values, { });

        stop_notifier();
        return result;
    }


    void refresh(const type_desc p_type, proc_pointer& p_pointer) const {
        refresh_value(p_type, p_pointer);
    }


    template <typename TypePointerGetter, typename TypeCollection>
    void refresh(const type_desc p_type, const TypePointerGetter& p_getter, TypeCollection& p_collection) const {
        if (m_force_parallel || p_collection.size() > TRIGGER_CPU_PARALLEL_ITEMS) {
            return read_parallel_cpu(p_getter, p_collection);
        }

        return read_sequentially(p_getter, p_collection);
    }


    void subscribe(const progress_observer& p_observer) {
        m_observer = p_observer;
    }


    void force_parallel_processing() {
        m_force_parallel = true;
    }

private:
    search_result combine_partial_results(std::vector<search_result>& partial_results) const {
        std::uint64_t length = 0;
        for (const auto& result : partial_results) {
            length += result.get_amount_memory_blocks();
        }

        search_result total_result(m_filter.get_type());
        total_result.reserve(length);
        for (auto& result : partial_results) {
            total_result.move(std::move(result));
            result.clear();
        }

        return total_result;
    }


    template <typename TypePointerGetter, typename TypeCollection>
    search_result refresh_sequentially(const proc_handle& p_handle, const type_desc p_type, const TypePointerGetter& p_getter, TypeCollection& p_collection) const {
        for (std::size_t i = 0; i < p_collection.size(); i++) {
            refresh_value(p_handle, p_type, p_getter(i));
        }
    }


    template <typename TypePointerGetter, typename TypeCollection>
    void refresh_parallel_cpu(const proc_handle& p_handle, const type_desc p_type, const TypePointerGetter& p_getter, TypeCollection& p_collection) const {
        auto task = [this, &p_handle, &p_type, &p_getter, &p_collection](const std::size_t p_idx) {
            refresh_value(p_handle, p_type, p_getter(p_idx));
        };

        crwproc::parallel::parallel_for(std::size_t(0), p_collection.size(), task);
    }


    void refresh_value(const type_desc p_type, proc_pointer& p_value) const {
        const std::size_t value_size = p_type.get_size();

        std::shared_ptr<std::uint8_t[]> buffer(new std::uint8_t[value_size]);
        std::uint8_t* raw_buffer = buffer.get();

        if (!ReadProcessMemory(m_proc_handle(), (LPCVOID)p_value.get_address(), (void *)raw_buffer, value_size, nullptr)) {
            p_value.invalidate();
            return;
        }

        return p_value.get_value().set((void *)raw_buffer, value_size);
    }


    search_result read_and_filter_eval(const search_result& p_values, const memblocks_info& p_info_blocks) const {
        switch (m_filter.get_type().get_type()) {
        case value_type::integral:
            switch (m_filter.get_type().get_size()) {
            case 1:
                return read_and_filter_with_type<std::uint8_t>(p_values, p_info_blocks);

            case 2:
                return read_and_filter_with_type<std::uint16_t>(p_values, p_info_blocks);

            case 4:
                return read_and_filter_with_type<std::uint32_t>(p_values, p_info_blocks);

            case 8:
                return read_and_filter_with_type<std::uint64_t>(p_values, p_info_blocks);

            default:
                throw std::logic_error("Invalid integer size '" + std::to_string(m_filter.get_type().get_size()) + "' is used by the filter.");
            }

        case value_type::floating:
            return read_and_filter_with_type<float>(p_values, p_info_blocks);

        case value_type::doubling:
            return read_and_filter_with_type<double>(p_values, p_info_blocks);

        default:
            throw std::logic_error("Unkown value type '" + std::to_string(static_cast<int>(m_filter.get_type().get_type())) + "' is used for filtering.");
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


    memblocks_info get_proc_memblocks() const {
        MEMORY_BASIC_INFORMATION  memory_info;

        std::uint64_t current_address = 0;

        memblocks_info result;

        while (VirtualQueryEx(m_proc_handle(), (LPCVOID)current_address, &memory_info, sizeof(memory_info))) {
            if (memory_info.State == MEM_COMMIT) {
                result.total_size += memory_info.RegionSize;
                result.blocks.push_back(memory_info);
            }

            current_address += memory_info.RegionSize;
        }

        return result;
    }


    std::uint64_t get_progress() const {
        if (m_bytes_to_read != 0) {
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

    /* initial search through the whole process memory */

    template <typename TypeValue>
    inline void read_and_filter_whole_process_iteration(const MEMORY_BASIC_INFORMATION& p_memory_info, search_result& p_result) const {
        std::shared_ptr<std::uint8_t[]> buffer(new std::uint8_t[p_memory_info.RegionSize]);
        std::memset(buffer.get(), 0x00, p_memory_info.RegionSize);

        std::uint64_t bytes_was_read = 0;

        if (ReadProcessMemory(m_proc_handle(), (LPCVOID)p_memory_info.BaseAddress, buffer.get(), p_memory_info.RegionSize, (SIZE_T*)&bytes_was_read)) {
            m_bytes_read += bytes_was_read;
            memory_block block = extract_values<TypeValue>(buffer, bytes_was_read, (std::uint64_t)p_memory_info.BaseAddress);
            if (!block.is_empty()) {
                p_result.get_memory_blocks().push_back(std::move(block));
            }
        }
    }


    template <typename TypeValue>
    search_result read_and_filter_whole_process_sequentially(const memblocks_info& p_info_blocks) const {
        search_result result(m_filter.get_type());

        const auto& info_blocks = p_info_blocks.blocks;

        for (const auto& memory_info : info_blocks) {
            read_and_filter_whole_process_iteration<TypeValue>(memory_info, result);
        }

        return result;
    }


    template <typename TypeValue>
    search_result read_and_filter_whole_process_parallel_cpu(const memblocks_info& p_info_blocks) const {
        std::vector<search_result> partial_results(crwproc::parallel::get_amount_threads(), search_result(m_filter.get_type()));

        const auto& info_blocks = p_info_blocks.blocks;

        auto task = [this, &info_blocks, &partial_results](const std::size_t p_idx, const std::size_t p_tidx) {
            const auto& memory_info = info_blocks[p_idx];
            read_and_filter_whole_process_iteration<TypeValue>(memory_info, partial_results[p_tidx]);
        };

        crwproc::parallel::parallel_for_with_tidx(std::size_t(0), info_blocks.size(), task);

        return combine_partial_results(partial_results);
    }


    /* search among previously founded values (previous search result) */


    inline std::shared_ptr<std::uint8_t[]> read_memory_for_block(const memory_block& p_block) const {
        const std::uint64_t region_size = p_block.get_region_size();
        std::shared_ptr<std::uint8_t[]> buffer(new std::uint8_t[region_size]);

        m_bytes_read += region_size;
        if (!ReadProcessMemory(m_proc_handle(), (LPCVOID)p_block.get_begin(), (void*)buffer.get(), region_size, nullptr)) {
#if 0
            std::cout << "[DEBUG] Block (" << (void*)p_block.get_begin() << "-" << (void*)p_block.get_end() << ") is not available due to " << GetLastError() << std::endl;

            MEMORY_BASIC_INFORMATION memory_info;
            VirtualQueryEx(m_proc_handle(), (LPCVOID)p_block.get_begin(), &memory_info, sizeof(memory_info));

            std::cout << "[DEBUG] Info  (" << memory_info.BaseAddress << "-" << (void*) ((std::uint64_t) memory_info.BaseAddress + memory_info.RegionSize) << ") " <<
                "protect = " << memory_info.Protect <<
                ", alloc_protect = " << memory_info.AllocationProtect << 
                ", state = " << memory_info.State << std::endl;
#endif
            return nullptr;
        }


        return buffer;
    }


    inline memory_block_sequence extract_dummy_blocks(const memory_block& p_block) const {
        MEMORY_BASIC_INFORMATION memory_info;
        std::memset(&memory_info, 0x00, sizeof(memory_info));

        memory_block_sequence result;
        for (std::uint64_t address = p_block.get_begin(); address < p_block.get_end(); address += memory_info.RegionSize) {
            if (!VirtualQueryEx(m_proc_handle(), (LPCVOID)address, &memory_info, sizeof(memory_info))) {
                return { };
            }

            if (memory_info.State == MEM_COMMIT) {  /* no need to consider block further if it was freed or reserved */
                const std::size_t address_end = std::min(address + memory_info.RegionSize, p_block.get_end());
                result.emplace_back(address, address_end);
            }
        }
        
        return result;
    }


    template <typename TypeValue>
    inline std::size_t read_and_filter_values_dummy_block(const proc_pointer_collection& p_values, const std::size_t p_values_index, memory_block& p_dummy) const {
        std::size_t index = p_values_index;

        std::shared_ptr<std::uint8_t[]> buffer = read_memory_for_block(p_dummy);
        if (buffer == nullptr) {
            /* skip values for this region */
            for (; (index < p_values.size()) && (p_values[index].get_address() < p_dummy.get_end()); index++) { }
            return index;
        }

        std::uint8_t* raw_buffer = buffer.get();

        for (; (index < p_values.size()) && (p_values[index].get_address() < p_dummy.get_end()); index++) {
            if (p_dummy.get_begin() > p_values[index].get_address()) {
                /* Memory block was not allocated for this value because it is located in non-commit state (MEM_FREE or MEM_RESERVE). */
                continue;
            }

            const std::uint64_t offset = p_values[index].get_address() - p_dummy.get_begin();

            const TypeValue actual_value = *((TypeValue*)(raw_buffer + offset));

            bool is_satisfying = false;
            if constexpr (crwproc::traits::is_any<TypeFilter, filter_more, filter_less>::value) {
                const TypeValue previous_value = p_values[index].get_value().get<TypeValue>();
                is_satisfying = m_filter.is_satisfying(actual_value, previous_value);
            }
            else {
                is_satisfying = m_filter.is_satisfying(actual_value);
            }

            if (is_satisfying) {
                p_dummy.add_value(proc_pointer::create(p_values[index].get_address(), actual_value));
            }
        }

        return index;
    }


    template <typename TypeValue>
    inline void read_and_filter_values_dummy_blocks(const memory_block& p_block, memory_block_sequence& p_dummy_blocks, search_result& p_result) const {
        std::size_t index_values = 0;
        for (auto& dummy_block : p_dummy_blocks) {
            index_values = read_and_filter_values_dummy_block<TypeValue>(p_block.get_values(), index_values, dummy_block);
            if (!dummy_block.is_empty()) {
                p_result.get_memory_blocks().push_back(std::move(dummy_block));
            }

            if (index_values >= p_block.get_values().size()) {
                return; /* all values are processed, no need to continue */
            }
        }
    }


    template <typename TypeValue>
    inline void read_and_filter_raw_memory_dummy_block(const memory_block& p_block, memory_block& p_dummy) const {
        const std::uint64_t region_size = p_dummy.get_region_size();
        std::shared_ptr<std::uint8_t[]> buffer = read_memory_for_block(p_dummy);
        if (buffer == nullptr) {
            return; /* skip values for this region */
        }

        std::uint8_t* raw_buffer = buffer.get();

        const std::size_t offset_last_value = region_size - sizeof(TypeValue) + 1;
        std::uint64_t offset_parent_block = p_dummy.get_begin() - p_block.get_begin();
        for (std::uint64_t offset = 0; offset < offset_last_value; ++offset, ++offset_parent_block) {
            const TypeValue actual_value = *((TypeValue*)(raw_buffer + offset));

            bool is_satisfying = false;
            if constexpr (crwproc::traits::is_any<TypeFilter, filter_more, filter_less>::value) {
                const TypeValue previous_value = *((TypeValue*)(p_block.get_memory().get() + offset_parent_block));
                is_satisfying = m_filter.is_satisfying(actual_value, previous_value);
            }
            else {
                is_satisfying = m_filter.is_satisfying(actual_value);
            }

            if (is_satisfying) {
                p_dummy.add_value(proc_pointer::create(p_dummy.get_begin() + offset, actual_value));
            }
        }
    }


    template <typename TypeValue>
    inline void read_and_filter_raw_memory_dummy_blocks(const memory_block& p_block, memory_block_sequence& p_dummy_blocks, search_result& p_result) const {
        for (auto& dummy_block : p_dummy_blocks) {
            read_and_filter_raw_memory_dummy_block<TypeValue>(p_block, dummy_block);
            if (!dummy_block.is_empty()) {
                p_result.get_memory_blocks().push_back(std::move(dummy_block));
            }
        }
    }


    template <typename TypeValue>
    inline void read_and_filter_values_iteration(const memory_block& p_block, search_result& p_result) const {
        /*
           Block size might be changed, for example, there was a block with size 4096, but inside of it the protection 
           has been changed from 4 to 1. It means that block has been divided into three blocks. Lets assume that it 
           was done for 1024 to 2048, in this case we will have three following blocks:
           1) 0 - 1024 with protection 4
           2) 1024 - 2048 with protection 1
           3) 2048 - 4096 - with protection 4 

           It means that a block might be splitted into two or more blocks.
        */

        memory_block_sequence dummy_blocks = extract_dummy_blocks(p_block);
        if (dummy_blocks.empty()) {
            return;
        }

        if (p_block.is_raw_memory()) {
            read_and_filter_raw_memory_dummy_blocks<TypeValue>(p_block, dummy_blocks, p_result);
        }
        else {
            read_and_filter_values_dummy_blocks<TypeValue>(p_block, dummy_blocks, p_result);
        }
    }


    template <typename TypeValue>
    search_result read_and_filter_values_sequentially(const search_result& p_previous_result) const {
        search_result result(m_filter.get_type());

        for (const auto& block : p_previous_result.get_memory_blocks()) {
            read_and_filter_values_iteration<TypeValue>(block, result);
        }

        return result;
    }


    template <typename TypeValue>
    search_result read_and_filter_values_parallel_cpu(const search_result& p_previous_result) const {
        std::vector<search_result> partial_results(crwproc::parallel::get_amount_threads(), search_result(m_filter.get_type()));

        const auto& blocks = p_previous_result.get_memory_blocks();
        crwproc::parallel::parallel_for_with_tidx(std::size_t(0), blocks.size(),
            [this, &blocks, &partial_results](const std::size_t p_idx, const std::size_t p_tidx) {
                read_and_filter_values_iteration<TypeValue>(blocks[p_idx], partial_results[p_tidx]);
            }
        );

        return combine_partial_results(partial_results);
    }


    template <typename TypeValue>
    search_result read_and_filter_with_type(const search_result& p_values, const memblocks_info& p_info_blocks) const {
        if (p_values.is_empty()) {
            if (m_force_parallel || (p_info_blocks.total_size >= TRIGGER_CPU_PARALLEL_BYTES)) {
                return read_and_filter_whole_process_parallel_cpu<TypeValue>(p_info_blocks);
            }

            return read_and_filter_whole_process_sequentially<TypeValue>(p_info_blocks);
        }

        if (m_force_parallel || (p_values.get_amount_values() >= TRIGGER_CPU_PARALLEL_ITEMS)) {
            return read_and_filter_values_parallel_cpu<TypeValue>(p_values);
        }

        return read_and_filter_values_sequentially<TypeValue>(p_values);
    }


    template <typename TypeValue>
    inline memory_block extract_values(const std::shared_ptr<std::uint8_t[]>& p_buffer, const std::uint64_t p_length, const std::uint64_t p_base_address) const {
        memory_block result(p_base_address, p_base_address + p_length);

        const std::uint8_t* buffer = (std::uint8_t*)p_buffer.get();

        if constexpr (crwproc::traits::is_any<TypeFilter, filter_more, filter_less>::value) {
            result.set_memory(p_buffer);  /* only capture current process memory in 'filter more' and 'filter less' */
        }
        else {
            const std::size_t offset_last_value = p_length - sizeof(TypeValue) + 1; /* prevent buffer out-of-range reading if value is bigger than 1 byte */
            for (std::uint64_t offset = 0; offset < offset_last_value; ++offset) {
                const TypeValue actual_value = *((TypeValue*)(buffer + offset));

                if (m_filter.is_satisfying(actual_value)) {
                    result.add_value(proc_pointer::create(p_base_address + offset, actual_value));
                }
            }
        }

        return result;
    }
};