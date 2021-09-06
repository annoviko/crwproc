/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once


#include <string>
#include <unordered_map>


enum class value_type {
    invalid,
    integral,
    floating,
    doubling
};


const std::unordered_map<std::string, value_type> STR_TYPE_DICT = {
    { "integral", value_type::integral },
    { "floating", value_type::floating },
    { "doubling", value_type::doubling }
};

