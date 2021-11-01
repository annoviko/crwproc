/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once


#include "edit_table.h"


class edit_table_change {
private:
    proc_info m_proc_info;
    edit_table m_entries;
    std::vector<std::string> m_changed_values;

public:
    edit_table_change() = default;

    edit_table_change(const proc_info& p_info);

public:
    void add(const edit_table_entry& p_entry);

    void revert();

    void clear();

    void set_proc_info(const proc_info& p_info);
};
