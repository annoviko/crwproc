/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "value_type.h"


const std::unordered_map<std::string, value_type> STR_VALUE_TYPE_DICT = {
    { "integral", value_type::integral },
    { "floating", value_type::floating },
    { "doubling", value_type::doubling }
};


const std::unordered_map<value_type, std::string> VALUE_TYPE_STR_DICT = {
    { value_type::integral, "integral" },
    { value_type::floating, "floating" },
    { value_type::doubling, "doubling" }
};


std::string value_type_to_string(const value_type p_type) {
    const auto iter = VALUE_TYPE_STR_DICT.find(p_type);
    if (iter == VALUE_TYPE_STR_DICT.cend()) {
        return "invalid";
    }

    return iter->second;
}


value_type string_to_value_type(const std::string& p_type) {
    const auto iter = STR_VALUE_TYPE_DICT.find(p_type);
    if (iter == STR_VALUE_TYPE_DICT.cend()) {
        return value_type::invalid;
    }

    return iter->second;
}


std::ostream& operator<<(std::ostream& p_stream, const value_type& p_type) {
    p_stream << value_type_to_string(p_type);
    return p_stream;
}
