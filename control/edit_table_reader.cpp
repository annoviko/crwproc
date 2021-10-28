/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "edit_table_reader.h"

#include <exception>
#include <filesystem>
#include <fstream>

#include <nlohmann/json.hpp>

#include "log/logging.h"

#include "deserializer.h"


edit_table_reader::edit_table_reader(const std::string& p_filename) : m_filename(p_filename)
{ }


bool edit_table_reader::read(edit_table& p_table) const {
    std::string p_content;
    if (load(p_content)) {
        return parse(p_content, p_table);
    }

    return false;
}


bool edit_table_reader::load(std::string& p_content) const {
    if (!std::filesystem::exists(m_filename)) {
        LOG_ERROR("Impossible to read edit table (reason: file '" << m_filename << "' does not exist).")
        return false;
    }

    try {
        std::ifstream stream(m_filename);

        std::string file_line;
        while(std::getline(stream, file_line)) {
            p_content += file_line;
        }

        stream.close();
    }
    catch (std::exception& p_exception) {
        LOG_ERROR("Impossible to read edit table from file '" << m_filename << "' (reason: '" << p_exception.what() << "').")
        return false;
    }

    return true;
}


bool edit_table_reader::parse(const std::string& p_content, edit_table& p_table) const {
    try {
        const nlohmann::json json_table = nlohmann::json::parse(p_content);
        deserializer::get(json_table, p_table);
    }
    catch(std::exception& p_exception) {
        LOG_ERROR("Impossible to parse edit table from file '" << m_filename << "' (reason: '" << p_exception.what() << "').")
        return false;
    }

    return true;
}
