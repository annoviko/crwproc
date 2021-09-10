/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "filter_range.h"


filter_range::filter_range(const type_desc& p_type) :
    filter_base(p_type),
    m_begin(),
    m_end()
{ }


filter_range::filter_range(const memory_value& p_begin, const memory_value& p_end, const type_desc& p_type) :
    filter_base(p_type),
    m_begin(p_begin),
    m_end(p_end)
{ }


std::ostream& operator<<(std::ostream& p_stream, const filter_range& p_filter) {
    p_stream << "FilterRange (from: " << p_filter.m_begin.to_string(p_filter.get_type()) << ", to: " << p_filter.m_end.to_string(p_filter.get_type()) << ")";
    return p_stream;
}