#pragma once

#include <functional>
#include <limits>
#include <vector>

#include "filter_value.h"
#include "handle.h"
#include "proc_pointer.h"
#include "proc_info.h"

#undef max


class proc_reader {
public:
    using read_observer = std::function<void(std::uint64_t, std::uint64_t, std::uint64_t)>;
    using filter_observer = std::function<void(std::uint64_t)>;

private:
    proc_info            m_proc_info;
    filter_value         m_filter;
    read_observer        m_reader_observer;
    filter_observer      m_filter_observer;

private:
    static constexpr std::uint64_t READ_VALUE_SIZE = 16;
    static constexpr std::uint64_t READ_WINDOW_SIZE = 1048576;  /* 1MB step */

    static constexpr std::uint64_t INVALID_PROC_SIZE = std::numeric_limits<std::uint64_t>::max();
    static constexpr std::uint64_t INVALID_INTEGER_VALUE = std::numeric_limits<std::uint64_t>::max();

public:
    proc_reader(const proc_info& p_info, const filter_value& p_filter);

public:
    proc_pointer_sequence read_and_filter() const;

    proc_pointer_sequence read_and_filter(const proc_pointer_sequence& p_values) const;

    proc_pointer_sequence read(proc_pointer_sequence& p_value);

    void set_read_observer(const read_observer& p_observer);

    void set_filter_observer(const filter_observer& p_observer);

private:
    std::uint64_t get_proc_size(const std::uint64_t p_pid) const;

    void extract_values(const std::uint8_t* p_buffer, const std::uint64_t p_length, proc_pointer_sequence& p_result) const;

    void extract_value(const std::uint8_t* p_buffer, proc_pointer_sequence& p_result) const;

    std::uint64_t extract_integral_value(const std::uint8_t* p_buffer) const;

    std::uint64_t get_value_size() const;

    void handle_observers(const std::uint64_t p_read_bytes, const std::uint64_t p_size, const std::uint64_t p_value_found) const;
};