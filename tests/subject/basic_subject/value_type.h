/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once


#include <string>


enum class value_type {
    i8 = 0,
    i16,
    i32,
    i64,
    u8,
    u16,
    u32,
    u64,
    flt,
    dbl,
    invalid
};


value_type get_value_type_from_string(const std::string& p_type);
