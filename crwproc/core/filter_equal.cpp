#include "filter_equal.h"


filter_equal::filter_equal(const value& p_value) :
    m_value(p_value)
{ }


bool filter_equal::is_valid() const {
    return m_value.is_valid();
}


bool filter_equal::is_satisfying(const std::string& p_value) const {
    try {
        switch (m_value.get_type()) {
        case value::type::doubling:
            return std::stod(p_value) == std::stod(m_value.get_value());

        case value::type::floating:
            return std::stof(p_value) == std::stof(m_value.get_value());

        default:
            return m_value.get_value() == p_value;
        }
    }
    catch (...) {
        return false;
    }
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
