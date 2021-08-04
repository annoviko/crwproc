#include "proc_info.h"

#include <limits>


const std::size_t proc_info::INVALID_PID = std::numeric_limits<std::size_t>::max();


proc_info::proc_info(const std::size_t p_pid, const std::string& p_name) :
    m_pid(p_pid), m_name(p_name)
{ }


const std::string& proc_info::name() const {
    return m_name;
}


const std::size_t proc_info::pid() const {
    return m_pid;
}


const bool proc_info::is_valid() const {
    return !(m_name.empty() || (m_pid == INVALID_PID));
}


std::ostream& operator<<(std::ostream& p_stream, const proc_info& p_info) {
    p_stream << "Process ID: " << p_info.m_pid << " | " << p_info.m_name;
    return p_stream;
}
