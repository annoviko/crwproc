/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once


#include <variant>


class base_log_event {
protected:
    std::size_t m_id = 0;

public:
    base_log_event() = default;
    explicit base_log_event(const std::size_t p_id) : m_id(p_id) { }

public:
    std::size_t get_id() const { return m_id; }
};


class log_event_flush : public base_log_event { 
public:
    log_event_flush() = default;
    explicit log_event_flush(const std::size_t p_id) : base_log_event(p_id) { }
};


class log_event_exit : public base_log_event {
public:
    log_event_exit() = default;
    explicit log_event_exit(const std::size_t p_id) : base_log_event(p_id) { }
};


class log_event_timeout : public base_log_event {
public:
    log_event_timeout() = default;
    explicit log_event_timeout(const std::size_t p_id) : base_log_event(p_id) { }
};


using log_event = std::variant<
    log_event_flush,
    log_event_exit,
    log_event_timeout
>;
