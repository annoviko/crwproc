#pragma once

#include "core/filter_equal.h"
#include "core/proc_pointer.h"
#include "core/proc_info.h"


class context {
private:
    proc_info               m_proc;
    filter_equal            m_filter;
    proc_pointer_sequence   m_found_values;
    proc_pointer_sequence   m_user_table;

public:
    void set_proc_info(const proc_info& p_proc);

    const proc_info& get_proc_info() const;

    void set_filter(const filter_equal& p_filter);

    const filter_equal& get_filter() const;

    filter_equal& get_filter();

    void set_values(const proc_pointer_sequence& p_container);

    const proc_pointer_sequence& get_found_values() const;

    proc_pointer_sequence& get_user_table();

    const proc_pointer_sequence& get_user_table() const;

public:
    friend std::ostream& operator<<(std::ostream& p_stream, const context& p_info);
};