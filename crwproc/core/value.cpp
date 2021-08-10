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


value::value(const value::type p_type, const std::size_t p_size, const void* p_buffer) :
    m_type(p_type),
    m_size(p_size)
{
    std::memset((void*)m_buffer, 0x00, sizeof(m_buffer));
    std::memcpy((void*)p_buffer, (void*)m_buffer, p_size);
}


value::value(const value::type p_type, const std::size_t p_size, const std::string& p_value) :
    m_type(p_type),
    m_size(p_size)
{
    std::memset((void*)m_buffer, 0x00, sizeof(m_buffer));
    set(p_value);
}


bool value::is_valid() const {
    return m_type != value::type::invalid;
}


std::uint64_t value::get_size() const {
    return m_size;
}


value::type value::get_type() const {
    return m_type;
}


std::string value::evaluate_string_value() const {
    switch (m_type) {
    case value::type::integral:
        switch (m_size) {
        case 1:
            return std::to_string(*((std::uint8_t*)m_buffer));

        case 2:
            return std::to_string(*((std::uint16_t*)m_buffer));

        case 4:
            return std::to_string(*((std::uint32_t*)m_buffer));

        case 8:
            return std::to_string(*((std::uint64_t*)m_buffer));
        }

    case value::type::floating:
        return std::to_string(*((float*)m_buffer));

    case value::type::doubling:
        return std::to_string(*((double*)m_buffer));
    }

    throw std::logic_error("Impossible to return value as a string due to unknown type.");
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
        return std::tolower(symbol);
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