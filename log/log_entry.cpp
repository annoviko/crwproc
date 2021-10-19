/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "log_entry.h"


log_entry::log_entry(const std::chrono::system_clock::time_point& p_time, const log_level p_level, const std::string& p_funcname, const std::size_t p_line, const std::size_t p_tid, const std::string p_message) :
    timestamp(p_time),
    level(p_level),
    funcname(p_funcname),
    line(p_line),
    tid(p_tid),
    message(p_message)
{ }
