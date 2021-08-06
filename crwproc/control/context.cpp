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


filter_value& context::get_filter() {
    return m_filter;
}


void context::set_values(const proc_pointer_sequence& p_container) {
    m_found_values = p_container;
}


const proc_pointer_sequence& context::get_found_values() const {
    return m_found_values;
}


std::ostream& operator<<(std::ostream& p_stream, const context& p_info) {
    if (p_info.get_proc_info().is_valid()) {
        std::cout << p_info.get_proc_info() << std::endl;
    }

    if (p_info.get_filter().is_valid()) {
        std::cout << p_info.get_filter() << std::endl;
    }

    std::cout << "Found values: " << p_info.get_found_values().size() << std::endl;

    return p_stream;
}