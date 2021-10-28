/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once


#include <string>

#include "edit_table.h"


class edit_table_reader {
private:
    std::string m_filename;

public:
    explicit edit_table_reader(const std::string& p_filename);

public:
    bool read(edit_table& p_table) const;

private:
    bool load(std::string& p_content) const;

    bool parse(const std::string& p_content, edit_table& p_table) const;
};
