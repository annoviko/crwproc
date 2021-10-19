/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once


#include <ostream>


enum class log_level {
    trace = 0,
    debug,
    info,
    warning,
    error,
    fatal,
    size
};


std::ostream& operator<<(std::ostream& p_stream, const log_level& p_level);
