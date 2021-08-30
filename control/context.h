/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once

#include "core/filter.h"
#include "core/proc_pointer.h"
#include "core/proc_info.h"


class context {
private:
    proc_info               m_proc;
    filter                  m_filter = filter_none{ };
    proc_pointer_sequence   m_found_values;
    proc_pointer_sequence   m_user_table;

public:
    void set_proc_info(const proc_info& p_proc);

    const proc_info& get_proc_info() const;

    const filter& get_filter() const;

    filter& get_filter();

    proc_pointer_sequence& get_found_values();

    const proc_pointer_sequence& get_found_values() const;

    proc_pointer_sequence& get_user_table();

    const proc_pointer_sequence& get_user_table() const;

public:
    friend std::ostream& operator<<(std::ostream& p_stream, const context& p_info);
};