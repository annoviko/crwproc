#pragma once

#include "core/proc_info.h"
#include "filter_value.h"


class context {
private:
    proc_info m_proc;
    filter_value m_filter;

public:
    void set_proc_info(const proc_info& p_proc);

    const proc_info& get_proc_info() const;

    void set_filter(const filter_value& p_filter);

    const filter_value& get_filter() const;

public:
    friend std::ostream& operator<<(std::ostream& p_stream, const context& p_info);
};