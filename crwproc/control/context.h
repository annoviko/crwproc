#pragma once

#include "core/filter_value.h"
#include "core/proc_pointer.h"
#include "core/proc_info.h"


class context {
private:
    proc_info               m_proc;
    filter_value            m_filter;
    proc_pointer_sequence   m_values;

public:
    void set_proc_info(const proc_info& p_proc);

    const proc_info& get_proc_info() const;

    void set_filter(const filter_value& p_filter);

    const filter_value& get_filter() const;

    void set_values(const proc_pointer_sequence& p_container);

    proc_pointer_sequence get_values() const;

public:
    friend std::ostream& operator<<(std::ostream& p_stream, const context& p_info);
};