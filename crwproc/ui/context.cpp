#include "context.h"


void context::set_proc_info(const proc_info& p_proc) {
    m_proc = p_proc;
}


const proc_info& context::get_proc_info() const {
    return m_proc;
}


void context::set_filter(const filter_value& p_filter) {
    m_filter = p_filter;
}


const filter_value& context::get_filter() const {
    return m_filter;
}


std::ostream& operator<<(std::ostream& p_stream, const context& p_info) {
    static const std::string line(40, '-');

    std::cout << std::endl << line << std::endl;
    if (p_info.get_proc_info().is_valid()) {
        std::cout << p_info.get_proc_info() << std::endl;
    }

    if (p_info.get_filter().is_valid()) {
        std::cout << p_info.get_filter() << std::endl;
    }

    std::cout << line << std::endl;
    return p_stream;
}