/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "edit_table_change.h"


edit_table_change::edit_table_change(const proc_info& p_info) :
    m_proc_info(p_info)
{ }


void edit_table_change::add(const edit_table_entry& p_entry) {
    m_entries.push_back(p_entry);
    m_entries.back().refresh(m_proc_info);

    const std::string value_before_change = m_entries.back().get_pointer().get_value().to_string(p_entry.get_type());

    m_changed_values.push_back(value_before_change);
}


void edit_table_change::revert() {
    for (std::size_t i = 0; i < m_entries.size(); i++) {
        m_entries[i].set_value(m_changed_values[i], m_proc_info);
    }

    m_entries.clear();
    m_changed_values.clear();
}


void edit_table_change::clear() {
    m_entries.clear();
    m_changed_values.clear();
}


std::size_t edit_table_change::size() const {
    return m_entries.size();
}


void edit_table_change::set_proc_info(const proc_info& p_info) {
    m_proc_info = p_info;
}
