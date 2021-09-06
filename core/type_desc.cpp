/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "type_desc.h"


type_desc::type_desc(const value_type p_type, const std::size_t p_size, const bool p_signed) :
    m_type(p_type),
    m_size(p_size),
    m_signed(p_signed)
{ }


bool type_desc::is_valid() const {
    return m_type == value_type::invalid;
}


value_type type_desc::get_type() const {
    return m_type;
}


std::size_t type_desc::get_size() const {
    return m_size;
}


bool type_desc::is_signed() const {
    return m_signed;
}
