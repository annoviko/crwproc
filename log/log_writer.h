/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once


#include <chrono>
#include <fstream>
#include <list>
#include <mutex>
#include <string>
#include <thread>

#include "log_entry.h"
#include "log_event.h"
#include "log_level.h"


class log_writer {
private:
    std::string m_filename;
    std::ofstream m_stream;

    std::thread m_writer_thread;

    std::mutex m_buffer_lock;
    std::list<log_entry> m_buffer;

    std::mutex m_event_lock;
    std::list<log_event> m_event_queue;
    std::condition_variable m_event_cv;

private:
    static constexpr std::size_t TRIGGER_FLUSH_LINES_AMOUNT = 100;
    static constexpr std::chrono::milliseconds TIMEOUT_MILLISECONDS = std::chrono::milliseconds(1000);

public:
    log_writer(const std::string& p_prefix);

    ~log_writer();

public:
    void log(const std::string& p_message, const log_level p_level, const std::string& p_filename, const std::size_t p_line);

    const std::string& get_filename() const;

private:
    std::string generate_filename(const std::string& p_prefix) const;

    std::string time_as_string(const std::chrono::system_clock::time_point& p_time, const char p_date_delim, const char p_delim, const char p_time_delim) const;

    std::size_t get_thread_id() const;

    void thread_writer();

    void flush();

    void send_request(const log_event& p_event);

    void send_flush_request();

    void send_exit_request();
};