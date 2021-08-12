#pragma once

#include <chrono>
#include <condition_variable>
#include <functional>
#include <limits>
#include <mutex>
#include <optional>
#include <ratio>
#include <thread>
#include <vector>

#include <windows.h>
#include <psapi.h>
#include <tlhelp32.h>

#include "filter_equal.h"
#include "handle.h"
#include "proc_pointer.h"
#include "proc_info.h"

#undef max


class proc_reader {
public:
    using progress_observer = std::function<void(std::size_t)>;

private:
    proc_info            m_proc_info;
    filter_equal         m_filter;

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

    static constexpr std::uint64_t INVALID_PROC_SIZE = std::numeric_limits<std::uint64_t>::max();
    static constexpr std::uint64_t INVALID_INTEGER_VALUE = std::numeric_limits<std::uint64_t>::max();

    static constexpr std::size_t   NOTIFICATION_PERIOD_MS = 200;

public:
    proc_reader(const proc_info& p_info, const filter_equal& p_filter);

    ~proc_reader();

public:
    proc_pointer_sequence read_and_filter() const;

    proc_pointer_sequence read_and_filter(const proc_pointer_sequence& p_values) const;

    proc_pointer_sequence read(const proc_pointer_sequence& p_values) const;

    void subscribe(const progress_observer& p_observer);

private:
    proc_pointer_sequence read_and_filter_eval(const handle& p_proc_handler, const proc_pointer_sequence& p_values = { }) const;

    std::uint64_t get_proc_size(const std::uint64_t p_pid) const;

    proc_pointer read_value(const handle& p_proc_handler, const std::uint64_t p_address, const value& p_value) const;

    std::uint64_t get_amount_bytes_to_read(const handle& proc_handler) const;

    std::uint64_t get_progress() const;

    void run_notifier() const;

    void stop_notifier() const;

    void notifier_thread() const;

private:
    template <typename TypeValue>
    proc_pointer_sequence read_and_filter_whole_process(const handle& p_handle) const {
        proc_pointer_sequence result;

        MEMORY_BASIC_INFORMATION  memory_info;
        std::uint64_t current_address = 0;

        while (VirtualQueryEx(p_handle(), (LPCVOID)current_address, &memory_info, sizeof(memory_info))) {
            if ((memory_info.State == MEM_COMMIT) && ((memory_info.Type == MEM_MAPPED) || (memory_info.Type == MEM_PRIVATE))) {
                std::shared_ptr<std::uint8_t[]> buffer(new std::uint8_t[memory_info.RegionSize]);
                std::memset(buffer.get(), 0x00, memory_info.RegionSize);

                std::uint64_t bytes_was_read = 0;

                if (ReadProcessMemory(p_handle(), (LPCVOID)memory_info.BaseAddress, buffer.get(), memory_info.RegionSize, (SIZE_T*)&bytes_was_read)) {
                    extract_values<TypeValue>(buffer.get(), bytes_was_read, (std::uint64_t)memory_info.BaseAddress, m_filter, result);
                }
            }

            current_address += memory_info.RegionSize;
        }

        return result;
    }


    template <typename TypeValue>
    proc_pointer_sequence read_and_filter_values(const handle& p_handle, const proc_pointer_sequence& p_values) const {
        proc_pointer_sequence result;

        for (const auto& pointer : p_values) {
            std::uint8_t buffer[READ_VALUE_SIZE];
            std::memset(buffer, 0x00, READ_VALUE_SIZE);

            const std::uint64_t bytes_to_read = pointer.get_value().get_size();
            std::uint64_t bytes_was_read = 0;

            if (!ReadProcessMemory(p_handle(), (LPCVOID)pointer.get_address(), buffer, bytes_to_read, (SIZE_T*)&bytes_was_read)) {
                continue;
            }

            const TypeValue actual_value = *((TypeValue*)(buffer));

            if (m_filter.is_satisfying(actual_value)) {
                result.push_back({ pointer.get_address(), m_filter.get_value() });
            }

            m_bytes_read += pointer.get_value().get_size();
        }

        return result;
    }


    template <typename TypeValue>
    proc_pointer_sequence read_and_filter_with_type(const handle& p_handle, const proc_pointer_sequence& p_values) const {
        if (p_values.empty()) {
            return read_and_filter_whole_process<TypeValue>(p_handle);
        }

        return read_and_filter_values<TypeValue>(p_handle, p_values);
    }


    template <typename TypeValue>
    void extract_values(const std::uint8_t* p_buffer, const std::uint64_t p_length, const std::uint64_t p_address, const filter_equal& p_filter, proc_pointer_sequence& p_result) const {
        for (std::uint64_t offset = 0; offset < p_length; offset++) {
            const TypeValue actual_value = *((TypeValue*)(p_buffer + offset));
            if (p_filter.is_satisfying(actual_value)) {
                p_result.emplace_back(p_address + offset, p_filter.get_value());
            }

            m_bytes_read++;
        }
    }
};