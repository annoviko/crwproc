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


value::value(const value::type p_type, const std::size_t p_size, const std::string& p_value) :
    m_type(p_type),
    m_size(p_size),
    m_value(p_value)
{ }


bool value::is_valid() const {
    return m_type != value::type::invalid;
}


std::uint64_t value::get_size() const {
    switch (m_type) {
    case value::type::integral:
        return m_size;

    case value::type::floating:
        return sizeof(float);

    case value::type::doubling:
        return sizeof(double);
    }

    throw std::logic_error("Unknown value type is detected '" + std::to_string(static_cast<std::uint64_t>(m_type)) + "')");
}


value::type value::get_type() const {
    return m_type;
}


const std::string& value::get_value() const {
    return m_value;
}


void value::set_value(const std::string& p_value) {
    m_value = p_value;
}


std::ostream& operator<<(std::ostream& p_stream, const value& p_info) {
    p_stream << "type: '" << value::TYPE_STR_DICT.at(p_info.get_type()) << "'";
    if (p_info.get_type() == value::type::integral) {
        p_stream << ", size: '" << p_info.get_size() << "'";
    }

    p_stream << ", value: '" << p_info.m_value << "'";
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