/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "filter_equal.h"


filter_equal::filter_equal(const value& p_value) :
    m_value(p_value)
{ }


bool filter_equal::is_valid() const {
    return m_value.is_valid();
}


bool filter_equal::is_value_signed() const {
    return m_value.is_signed();
}


value::type filter_equal::get_value_type() const {
    return m_value.get_type();
}


std::size_t filter_equal::get_value_size() const {
    return m_value.get_size();
}


std::ostream& operator<<(std::ostream& p_stream, const filter_equal& p_info) {
    p_stream << "FilterEqual (" << p_info.m_value << ")";
    return p_stream;
}
