/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once


#include <string>


enum class change_type {
    add = 0,
    sub,
    set,
    invalid
};


change_type get_change_type_from_string(const std::string& p_type);
