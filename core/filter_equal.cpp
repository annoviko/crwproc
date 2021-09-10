/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "filter_equal.h"


filter_equal::filter_equal(const type_desc& p_type) :
    filter_base(p_type),
    m_value()
{ }


filter_equal::filter_equal(const memory_value& p_value, const type_desc& p_type) :
    filter_base(p_type),
    m_value(p_value)
{ }


std::ostream& operator<<(std::ostream& p_stream, const filter_equal& p_filter) {
    p_stream << "FilterEqual (" << p_filter.m_value.to_string(p_filter.get_type()) << ")";
    return p_stream;
}
