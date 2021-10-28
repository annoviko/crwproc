/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once


#include <nlohmann/json.hpp>

#include "edit_table.h"
#include "edit_table_entry.h"


class deserializer {
public:
    static void get(const nlohmann::json& p_json, type_desc& p_object);

    static void get(const nlohmann::json& p_json, edit_table_entry& p_object);

    static void get(const nlohmann::json& p_json, edit_table& p_object);
};