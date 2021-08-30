/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "filter_range.h"


filter_range::filter_range(const value& p_begin, const value& p_end) :
    m_begin(p_begin),
    m_end(p_end)
{ }


bool filter_range::is_valid() const {
    return m_begin.is_valid() && m_end.is_valid() && (m_begin.get_type() == m_end.get_type()) && (m_begin.get_size() == m_end.get_size());
}


bool filter_range::is_value_signed() const {
    return m_begin.is_signed();
}


value::type filter_range::get_value_type() const {
    return m_begin.get_type();
}


std::size_t filter_range::get_value_size() const {
    return m_begin.get_size();
}


std::ostream& operator<<(std::ostream& p_stream, const filter_range& p_info) {
    p_stream << "FilterRange (from: " << p_info.m_begin << ", to: " << p_info.m_end << ")";
    return p_stream;
}