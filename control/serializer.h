/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once


#include <nlohmann/json.hpp>

#include "core/type_desc.h"

#include "edit_table.h"
#include "edit_table_entry.h"


class serializer {
public:
    static nlohmann::json get(const type_desc& p_object);

    static nlohmann::json get(const edit_table_entry& p_object);

    static nlohmann::json get(const edit_table& p_object);
};