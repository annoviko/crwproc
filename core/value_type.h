/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once


#include <iostream>
#include <string>
#include <unordered_map>


enum class value_type {
    invalid,
    integral,
    floating,
    doubling
};


extern const std::unordered_map<std::string, value_type> STR_VALUE_TYPE_DICT;

extern const std::unordered_map<value_type, std::string> VALUE_TYPE_STR_DICT;


std::string value_type_to_string(const value_type p_type);

value_type string_to_value_type(const std::string& p_type);


std::ostream& operator<<(std::ostream& p_stream, const value_type& p_type);
