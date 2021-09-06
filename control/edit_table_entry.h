/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once


#include <iostream>
#include <string>

#include "core/proc_pointer.h"
#include "core/type_desc.h"


class edit_table_entry {
private:
    std::string     m_name;
    proc_pointer    m_pointer;
    type_desc       m_type;

public:
    edit_table_entry() = default;

    edit_table_entry(const std::string& p_name, const proc_pointer& p_pointer, const type_desc& p_type);

    edit_table_entry(const proc_pointer& p_pointer, const type_desc& p_type);

public:
    void set_name(const std::string& p_name);

    void set_value(const std::string& p_value);

    void refresh();

public:
    friend std::ostream& operator<<(std::ostream& p_stream, const edit_table_entry& p_entry);
};