/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "serializer.h"


nlohmann::json serializer::get(const type_desc& p_object) {
    nlohmann::json json_object = nlohmann::json::object();
    json_object["type"] = value_type_to_string(p_object.get_type());
    json_object["size"] = p_object.get_size();
    json_object["signed"] = p_object.is_signed();

    return json_object;
}


nlohmann::json serializer::get(const edit_table_entry& p_object) {
    nlohmann::json json_object = nlohmann::json::object();
    json_object["name"] = p_object.get_name();
    json_object["address"] = p_object.get_pointer().get_address();
    json_object["type"] = get(p_object.get_type());

    return json_object;
}


nlohmann::json serializer::get(const edit_table& p_object) {
    nlohmann::json json_array = nlohmann::json::array();
    for (const auto& entry : p_object) {
        json_array.push_back(get(entry));
    }

    return json_array;
}
