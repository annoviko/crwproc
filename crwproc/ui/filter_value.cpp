#include "filter_value.h"

#include <algorithm>
#include <cctype>
#include <iterator>


const std::unordered_map<std::string, filter_value::type> filter_value::STR_TYPE_DICT = {
    { "integral", filter_value::type::integral },
    { "floating", filter_value::type::floating },
    { "doubling", filter_value::type::doubling }
};


const std::unordered_map<filter_value::type, std::string> filter_value::TYPE_STR_DICT = {
    { filter_value::type::integral, "integral" },
    { filter_value::type::floating, "floating" },
    { filter_value::type::doubling, "doubling" }
};


filter_value::filter_value(const type p_type, const std::size_t p_size, const std::string& p_value) :
    m_type(p_type),
    m_size(p_size),
    m_value(p_value)
{ }


bool filter_value::is_valid() const {
    return m_type != filter_value::type::invalid;
}


filter_value::type filter_value::get_type() const {
    return m_type;
}


std::size_t filter_value::get_size() const {
    return m_size;
}


std::ostream& operator<<(std::ostream& p_stream, const filter_value& p_info) {
    p_stream << "Filter (type: " << filter_value::TYPE_STR_DICT.at(p_info.get_type());
    if (p_info.get_type() == filter_value::type::integral) {
        p_stream << ", size: " << p_info.get_size();
    }

    p_stream << ", value: " << p_info.m_value << ").";
    return p_stream;
}


filter_value::type filter_value::to_type(const std::string& p_type) {
    std::string canonical_type;
    std::transform(p_type.begin(), p_type.end(), std::back_inserter(canonical_type), [](const char symbol) {
        return std::tolower(symbol);
    });

    const auto iter = STR_TYPE_DICT.find(canonical_type);
    if (iter == STR_TYPE_DICT.cend()) {
        return filter_value::type::invalid;
    }

    return iter->second;
}