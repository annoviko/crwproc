/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "proc_pointer.h"


proc_pointer::proc_pointer(const std::uint64_t p_address, const void* p_buffer, const std::size_t p_size) :
    m_address(p_address),
    m_value(p_buffer, p_size)
{ }


proc_pointer::proc_pointer(const std::uint64_t p_address) :
    m_address(p_address)
{ }


bool proc_pointer::is_valid() const {
    return m_address == INVALID_ADDRESS;
}


void proc_pointer::invalidate() {
    m_address = INVALID_ADDRESS;
}


std::uint64_t proc_pointer::get_address() const {
    return m_address;
}


const memory_value& proc_pointer::get_value() const {
    return m_value;
}


memory_value& proc_pointer::get_value() {
    return m_value;
}


bool proc_pointer::operator==(const proc_pointer& p_pointer) const {
    return m_address == p_pointer.m_address;
}
