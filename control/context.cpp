/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "context.h"


void context::set_proc_info(const proc_info& p_proc) {
    m_proc = p_proc;
    m_user_table_change.set_proc_info(m_proc);
}


const proc_info& context::get_proc_info() const {
    return m_proc;
}


filter& context::get_filter() {
    return m_filter;
}


const filter& context::get_filter() const {
    return m_filter;
}


search_result& context::get_found_values() {
    return m_found_values;
}


const search_result& context::get_found_values() const {
    return m_found_values;
}


edit_table& context::get_user_table() {
    return m_user_table;
}


const edit_table& context::get_user_table() const {
    return m_user_table;
}


edit_table_names& context::get_user_table_active_names() {
    return m_user_table_active_names;
}


const edit_table_names& context::get_user_table_active_names() const {
    return m_user_table_active_names;
}


edit_table_change& context::get_user_table_change() {
    return m_user_table_change;
}


const edit_table_change& context::get_user_table_change() const {
    return m_user_table_change;
}


std::ostream& operator<<(std::ostream& p_stream, const context& p_info) {
    if (p_info.get_proc_info().is_valid()) {
        std::cout << p_info.get_proc_info() << std::endl;
    }

    std::cout << p_info.get_filter() << std::endl;

    std::cout << "Found values: " << p_info.get_found_values().get_amount_values() << std::endl;
    std::cout << "Saved values: " << p_info.get_user_table().size() << std::endl;

    return p_stream;
}