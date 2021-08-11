#include "filter_equal.h"


filter_equal::filter_equal(const value& p_value) :
    m_value(p_value)
{ }


bool filter_equal::is_valid() const {
    return m_value.is_valid();
}


value& filter_equal::get_value() {
    return m_value;
}


const value& filter_equal::get_value() const {
    return m_value;
}


std::ostream& operator<<(std::ostream& p_stream, const filter_equal& p_info) {
    p_stream << "FilterEqual (" << p_info.get_value() << ")";
    return p_stream;
}
