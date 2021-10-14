#include "change_type.h"

#include <unordered_map>


std::unordered_map<std::string, change_type> DICT_CHANGE_TYPE = {
    { "add", change_type::add },
    { "sub", change_type::sub },
    { "set", change_type::set }
};


change_type get_change_type_from_string(const std::string& p_type) {
    const auto iter = DICT_CHANGE_TYPE.find(p_type);
    if (iter != DICT_CHANGE_TYPE.cend()) {
        return iter->second;
    }

    return change_type::invalid;
}
