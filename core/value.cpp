/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "value.h"

#include <algorithm>
#include <iterator>
#include <exception>


const std::unordered_map<std::string, value::type> value::STR_TYPE_DICT = {
    { "integral", value::type::integral },
    { "floating", value::type::floating },
    { "doubling", value::type::doubling }
};


const std::unordered_map<value::type, std::string> value::TYPE_STR_DICT = {
    { value::type::integral, "integral" },
    { value::type::floating, "floating" },
    { value::type::doubling, "doubling" }
};


value::value(const value::type p_type, const std::size_t p_size, const bool p_signed, const void* p_buffer) :
    m_type(p_type),
    m_size(p_size),
    m_signed(p_signed)
{
    std::memset((void*)m_buffer, 0x00, sizeof(m_buffer));
    std::memcpy((void*)m_buffer, (void*)p_buffer, p_size);
}


value::value(const value::type p_type, const std::size_t p_size, const bool p_signed, const std::string& p_value) :
    m_type(p_type),
    m_size(p_size),
    m_signed(p_signed),
    m_value_string_valid(true)
{
    std::memset((void*)m_buffer, 0x00, sizeof(m_buffer));
    set(p_value);
}


value::value(const value::type p_type, const std::size_t p_size, const bool p_signed) :
    value(p_type, p_size, p_signed, "0")
{ }


bool value::is_valid() const {
    return m_type != value::type::invalid;
}


bool value::is_signed() const {
    return m_signed;
}


std::size_t value::get_size() const {
    return m_size;
}


value::type value::get_type() const {
    return m_type;
}


void value::update_string_value() const {
    if (!m_value_string_valid) {
        m_value_as_string = evaluate_string_value();
    }
}


std::string value::evaluate_string_value() const {
    switch (m_type) {
    case value::type::integral:
        switch (m_size) {
        case 1:
            return m_signed ? std::to_string(*((std::int8_t*)m_buffer)) : std::to_string(*((std::uint8_t*)m_buffer));

        case 2:
            return m_signed ? std::to_string(*((std::int16_t*)m_buffer)) : std::to_string(*((std::uint16_t*)m_buffer));

        case 4:
            return m_signed ? std::to_string(*((std::int32_t*)m_buffer)) : std::to_string(*((std::uint32_t*)m_buffer));

        case 8:
            return m_signed ? std::to_string(*((std::int64_t*)m_buffer)) : std::to_string(*((std::uint64_t*)m_buffer));

        default:
            return "invalid";
        }

    case value::type::floating:
        return std::to_string(*((float*)((void*)m_buffer)));

    case value::type::doubling:
        return std::to_string(*((double*)((void*)m_buffer)));

    default:
        return "invalid";
    }
}


std::ostream& operator<<(std::ostream& p_stream, const value& p_info) {
    p_stream << "type: '" << value::TYPE_STR_DICT.at(p_info.get_type()) << "'";
    if (p_info.get_type() == value::type::integral) {
        p_stream << ", size: '" << p_info.get_size() << "'";
    }

    p_stream << ", value: '" << p_info.get<std::string>() << "'";
    return p_stream;
}


value::type value::string_to_type(const std::string& p_type) {
    std::string canonical_type;
    std::transform(p_type.begin(), p_type.end(), std::back_inserter(canonical_type), [](const char symbol) {
        return static_cast<char>(std::tolower(symbol));
    });

    const auto iter = STR_TYPE_DICT.find(canonical_type);
    if (iter == STR_TYPE_DICT.cend()) {
        return value::type::invalid;
    }

    return iter->second;
}


std::string value::type_to_string(const value::type p_type) {
    const auto iter = TYPE_STR_DICT.find(p_type);
    if (iter == TYPE_STR_DICT.cend()) {
        return "invalid";
    }

    return iter->second;
}