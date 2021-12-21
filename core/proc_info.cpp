/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "proc_info.h"

#include "proc_handle.h"

#include <limits>


#undef max


const std::size_t proc_info::INVALID_PID = std::numeric_limits<std::size_t>::max();


proc_info::proc_info(const std::size_t p_pid, const std::string& p_name, const std::uint64_t p_address) :
    m_pid(p_pid), 
    m_name(p_name), 
    m_address(p_address)
{ }


const std::string& proc_info::name() const {
    return m_name;
}


std::size_t proc_info::pid() const {
    return m_pid;
}


std::uint64_t proc_info::base_address() const {
    return m_address;
}


bool proc_info::is_valid() const {
    return !(m_name.empty() || (m_pid == INVALID_PID));
}


bool proc_info::is_running() const {
    const proc_handle handle = proc_handle(m_pid, proc_handle::access::read);
    if (handle() == nullptr) {
        return false;
    }

    return handle.is_running();
}


std::ostream& operator<<(std::ostream& p_stream, const proc_info& p_info) {
    if (p_info.is_running()) {
        p_stream << "Process ID: " << p_info.m_pid << " | " << p_info.m_name;
    }
    else {
        p_stream << "Process ID: [TERMINATED] | " << p_info.m_name;
    }
    
    return p_stream;
}
