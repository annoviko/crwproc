/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "filter_more.h"


filter_more::filter_more(const value& p_value) : 
    m_blank_value(p_value)
{ }


bool filter_more::is_valid() const {
    return m_blank_value.is_valid();
}


bool filter_more::is_value_signed() const {
    return m_blank_value.is_signed();
}


value::type filter_more::get_value_type() const {
    return m_blank_value.get_type();
}


std::size_t filter_more::get_value_size() const {
    return m_blank_value.get_size();
}


std::ostream& operator<<(std::ostream& p_stream, const filter_more& p_info) {
    p_stream << "FilterMore (x[t] > x[t-1])";
    return p_stream;
}
