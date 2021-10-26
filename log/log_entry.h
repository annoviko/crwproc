/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once


#include <chrono>
#include <string>
#include <thread>

#include "log_level.h"


struct log_entry {
public:
    std::chrono::system_clock::time_point timestamp;
    log_level       level;
    std::string     filename;
    std::size_t     line;
    std::size_t     tid;
    std::string     message;

public:
    log_entry(const std::chrono::system_clock::time_point& p_time, const log_level p_level, const std::string& p_filename, const std::size_t p_line, const std::size_t p_tid, const std::string& p_message);
};
