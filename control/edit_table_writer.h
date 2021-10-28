/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once


#include <string>

#include "edit_table.h"


class edit_table_writer {
private:
    std::string m_filename;

public:
    explicit edit_table_writer(const std::string& p_filename);

public:
    bool write(const edit_table& p_table) const;
};
