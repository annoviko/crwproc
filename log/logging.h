/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once


#include <sstream>

#include "log_writer.h"


class logging {
public:
    logging() = delete;

private:
    static std::string PREFIX;

public:
    static void initialize(const std::string& p_prefix);

    static log_writer& instance();
};


#define _LOG(_message, _level, _funcname, _line) {                          \
    std::stringstream _stream;                                              \
    _stream << _message;                                                    \
    logging::instance().log(_stream.str(), _level, _funcname, _line);       \
}


#define LOG_TRACE(_message)         _LOG(_message, log_level::trace, __FUNCTION__, __LINE__)
#define LOG_DEBUG(_message)         _LOG(_message, log_level::debug, __FUNCTION__, __LINE__)
#define LOG_INFO(_message)          _LOG(_message, log_level::info, __FUNCTION__, __LINE__)
#define LOG_WARNING(_message)       _LOG(_message, log_level::warning, __FUNCTION__, __LINE__)
#define LOG_ERROR(_message)         _LOG(_message, log_level::error, __FUNCTION__, __LINE__)
#define LOG_FATAL(_message)         _LOG(_message, log_level::fatal, __FUNCTION__, __LINE__)
