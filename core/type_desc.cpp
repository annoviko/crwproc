/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "type_desc.h"


type_desc::type_desc(const value_type p_type, const std::size_t p_size, const bool p_signed) :
    m_type(p_type),
    m_size(p_size),
    m_signed(p_signed)
{ 
    switch (m_type) {
    case value_type::doubling:
        m_size = sizeof(double);
        m_signed = true;
        break;

    case value_type::floating:
        m_size = sizeof(float);
        m_signed = true;
        break;

    default:
        break;
    }
}


bool type_desc::is_valid() const {
    return m_type != value_type::invalid;
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


std::string type_desc::to_string() const {
    switch (m_type) {
        case value_type::integral:
            switch (m_size) {
                case 1: return m_signed ? "int8_t" : "uint8_t";
                case 2: return m_signed ? "int16_t" : "uint16_t";
                case 4: return m_signed ? "int32_t" : "uint32_t";
                case 8: return m_signed ? "int64_t" : "uint64_t";
                default: return "invalid";
            }

        case value_type::doubling:
            return "double";

        case value_type::floating:
            return "float";

        default:
            return "invalid";
    }
}


bool type_desc::operator==(const type_desc& p_type) const {
    return (m_type == p_type.m_type) &&
        (m_size == p_type.m_size) &&
        (m_signed == p_type.m_signed);
}


std::ostream& operator<<(std::ostream& p_stream, const type_desc& p_type) {
    p_stream << p_type.to_string();
    return p_stream;
}
