#pragma once

#include "core/proc_info.h"


class context {
private:
    proc_info m_proc;

public:
    void set_proc_info(const proc_info& p_proc);

    const proc_info& get_proc_info() const;
};