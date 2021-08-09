#include "proc_pointer.h"


proc_pointer::proc_pointer(const std::uint64_t p_address, const value& p_value) :
    m_address(p_address),
    m_value(p_value)
{ }


bool proc_pointer::is_valid() const {
    return m_value.is_valid();
}


std::uint64_t proc_pointer::get_address() const {
    return m_address;
}


value& proc_pointer::get_value() {
    return m_value;
}


const value& proc_pointer::get_value() const {
    return m_value;
}
