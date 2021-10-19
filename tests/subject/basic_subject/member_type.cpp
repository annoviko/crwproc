/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "memory_type.h"

#include <unordered_map>


std::unordered_map<std::string, memory_type> DICT_MEMORY_TYPE = {
    { "global", memory_type::global },
    { "heap", memory_type::heap },
    { "stack", memory_type::stack }
};


memory_type get_memory_type_from_string(const std::string& p_type) {
    const auto iter = DICT_MEMORY_TYPE.find(p_type);
    if (iter != DICT_MEMORY_TYPE.cend()) {
        return iter->second;
    }

    return memory_type::invalid;
}
