#include "context.h"


void context::set_proc_info(const proc_info& p_proc) {
    m_proc = p_proc;
}


const proc_info& context::get_proc_info() const {
    return m_proc;
}
