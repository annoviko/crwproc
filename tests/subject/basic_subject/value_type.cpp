/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "value_type.h"

#include <unordered_map>


std::unordered_map<std::string, value_type> DICT_VALUE_TYPE = {
    { "i8",  value_type::i8  },
    { "i16", value_type::i16 },
    { "i32", value_type::i32 },
    { "i64", value_type::i64 },
    { "u8",  value_type::u8  },
    { "u16", value_type::u16 },
    { "u32", value_type::u32 },
    { "u64", value_type::u64 },
    { "flt", value_type::flt },
    { "dbl", value_type::dbl }
};


value_type get_value_type_from_string(const std::string& p_type) {
    const auto iter = DICT_VALUE_TYPE.find(p_type);
    if (iter != DICT_VALUE_TYPE.cend()) {
        return iter->second;
    }

    return value_type::invalid;
}
