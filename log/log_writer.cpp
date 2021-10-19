/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "log_writer.h"

#include <iomanip>
#include <sstream>
#include <time.h>

#include <windows.h>
#include <processthreadsapi.h>


log_writer::log_writer(const std::string& p_prefix) :
    m_filename(generate_filename(p_prefix)),
    m_stream(m_filename, std::ios_base::app),
    m_writer_thread(&log_writer::thread_writer, this)
{ }


log_writer::~log_writer() {
    if (!m_writer_thread.joinable()) {
        return;
    }

    {
        std::unique_lock<std::mutex> queue_guard(m_event_lock);
        m_event_queue.push_back(log_event_exit{ });
    }

    m_event_cv.notify_one();
    m_writer_thread.join();
}


void log_writer::log(const std::string& p_message, const log_level p_level, const std::string& p_funcname, const std::size_t p_line) {
    const auto start_time = std::chrono::system_clock::now();
    const std::size_t tid = get_thread_id();

    std::lock_guard<std::mutex> guard(m_buffer_lock);
    m_buffer.emplace_back(start_time, p_level, p_funcname, p_line, tid, p_message);

    if (m_buffer.size() > TRIGGER_FLUSH_LINES_AMOUNT) {
        send_flush_request();
    }
}


const std::string& log_writer::get_filename() const {
    return m_filename;
}


std::string log_writer::generate_filename(const std::string& p_prefix) const {
    const auto start_time = std::chrono::system_clock::now();

    std::stringstream stream;
    stream << std::setfill('0') << p_prefix << "_"
        << time_as_string(start_time, '-', '_', '-') << ".log";

    return stream.str();
}


std::string log_writer::time_as_string(const std::chrono::system_clock::time_point& p_time, const char p_date_delim, const char p_delim, const char p_time_delim) const {
    std::time_t current_time = std::chrono::system_clock::to_time_t(p_time);

    tm time_info;
    localtime_s(&time_info, &current_time);

    std::stringstream stream;
    stream << std::setfill('0')
        << time_info.tm_year + 1900 << p_date_delim
        << std::setw(2) << time_info.tm_mon << p_date_delim
        << std::setw(2) << time_info.tm_mday << p_delim
        << std::setw(2) << time_info.tm_hour << p_time_delim
        << std::setw(2) << time_info.tm_min << p_time_delim
        << std::setw(2) << time_info.tm_sec;

    return stream.str();
}


std::size_t log_writer::get_thread_id() const {
    return static_cast<std::size_t>(GetCurrentThreadId());  /* faster than std::stringstream + std::this_thread::id */
}


void log_writer::thread_writer() {
    bool is_running = true;
    while (is_running) {
        try {
            log_event to_process = log_event_timeout{ };

            {
                std::unique_lock<std::mutex> queue_guard(m_event_lock);

                /* do not care much about spurious awakenings - just flush what we have in buffer instead of waiting for precise period */
                if (m_event_cv.wait_for(queue_guard, TIMEOUT_MILLISECONDS, [this]() { return !m_event_queue.empty(); })) {
                    to_process = m_event_queue.front();
                    m_event_queue.pop_front();
                }
            }

            std::visit([this, &is_running](auto&& instance) {
                using EventType = std::decay_t<decltype(instance)>;

                if constexpr (std::is_same_v<EventType, log_event_flush>) {
                    flush();
                }
                else if (std::is_same_v<EventType, log_event_timeout>) {
                    flush();
                }
                else if (std::is_same_v<EventType, log_event_exit>) {
                    flush();
                    is_running = false;
                }
            }, to_process);
        }
        catch (...) { }
    }
}


void log_writer::flush() {
    std::list<log_entry> buffer_to_write;

    {
        std::lock_guard<std::mutex> guard(m_buffer_lock);
        buffer_to_write = std::move(m_buffer);
    }

    for (const auto& entry : buffer_to_write) {
        m_stream
            << time_as_string(entry.timestamp, '-', ' ', ':') << " "
            << entry.level << " "
            << entry.funcname << ":" << entry.line
            << " [" << entry.tid << "] "
            << entry.message
            << std::endl;
    }
}


void log_writer::send_request(const log_event& p_event) {
    {
        std::lock_guard<std::mutex> guard(m_event_lock);
        m_event_queue.push_back(p_event);
    }

    m_event_cv.notify_one();
}


void log_writer::send_flush_request() {
    send_request(log_event_flush{ });
}


void log_writer::send_exit_request() {
    send_request(log_event_exit{ });
}
