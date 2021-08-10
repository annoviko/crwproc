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

public:
    proc_pointer_sequence read_and_filter() const;

    proc_pointer_sequence read_and_filter(const proc_pointer_sequence& p_values) const;

    proc_pointer_sequence read(const proc_pointer_sequence& p_values) const;

    void subscribe(const progress_observer& p_observer);

private:
    std::uint64_t get_proc_size(const std::uint64_t p_pid) const;

    proc_pointer read_value(const handle& p_proc_handler, const std::uint64_t p_address, const value& p_value) const;

    void extract_values(const std::uint8_t* p_buffer, const std::uint64_t p_length, const std::uint64_t p_address, const value& p_value, const bool p_filter, proc_pointer_sequence& p_result) const;

    proc_pointer extract_value(const std::uint8_t* p_buffer, const std::uint64_t p_address, const value& p_value) const;

    std::uint64_t extract_integral_value(const std::uint8_t* p_buffer, const std::size_t p_size) const;

    std::uint64_t get_amount_bytes_to_read(const handle& proc_handler) const;

    std::uint64_t get_progress() const;

    void run_notifier() const;

    void stop_notifier() const;

    void notifier_thread() const;
};