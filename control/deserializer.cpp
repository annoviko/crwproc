/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "deserializer.h"


void deserializer::get(const nlohmann::json& p_json, type_desc& p_object) {
    std::string val = p_json.dump();

    const std::string type = p_json["type"].get<std::string>();
    const std::size_t size = p_json["size"].get<std::size_t>();
    const bool is_signed = p_json["signed"].get<bool>();

    p_object = type_desc(string_to_value_type(type), size, is_signed);
}


void deserializer::get(const nlohmann::json& p_json, edit_table_entry& p_object) {
    std::string val = p_json.dump();

    const std::string name = p_json["name"].get<std::string>();
    const std::uint64_t address = p_json["address"].get<std::uint64_t>();

    type_desc type;
    get(p_json["type"], type);

    p_object = edit_table_entry(name, proc_pointer(address), type);
}


void deserializer::get(const nlohmann::json& p_json, edit_table& p_object) {
    for (auto iter = p_json.begin(); iter != p_json.end(); ++iter) {
        edit_table_entry entry;
        get(*iter, entry);

        p_object.push_back(entry);
    }
}
