/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once

#include "core/filter.h"
#include "core/proc_pointer.h"
#include "core/proc_info.h"
#include "core/search_result.h"

#include "edit_table.h"
#include "edit_table_change.h"


class context {
private:
    proc_info         m_proc;
    filter            m_filter = filter_none{ };
    search_result     m_found_values;
    edit_table        m_user_table;
    edit_table_change m_user_table_change;

public:
    void set_proc_info(const proc_info& p_proc);

    const proc_info& get_proc_info() const;

    const filter& get_filter() const;

    filter& get_filter();

    search_result& get_found_values();

    const search_result& get_found_values() const;

    edit_table& get_user_table();

    const edit_table& get_user_table() const;

    edit_table_change& get_user_table_change();

    const edit_table_change& get_user_table_change() const;

public:
    friend std::ostream& operator<<(std::ostream& p_stream, const context& p_info);
};