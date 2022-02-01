/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "proc_handle.h"

#include <string>
#include <stdexcept>


proc_handle::proc_handle(const std::size_t p_pid, const access p_access) {
    switch (p_access) {
    case access::read:
        m_handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, static_cast<DWORD>(p_pid));
        break;

    case access::write:
        m_handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_WRITE, FALSE, static_cast<DWORD>(p_pid));
        break;

    default:
        throw std::invalid_argument("Unknown access is specified '" + std::to_string(static_cast<std::size_t>(p_access)) + "'.");
    }
}


proc_handle::~proc_handle() {
    if (m_handle != nullptr) {
        CloseHandle(m_handle);
    }
}


bool proc_handle::is_running() const {
    DWORD status = 0;
    GetExitCodeProcess(m_handle, &status);

    return (status == STILL_ACTIVE);
}


HANDLE proc_handle::operator()() const {
    return m_handle;
}
