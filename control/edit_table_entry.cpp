/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "edit_table_entry.h"


edit_table_entry::edit_table_entry(const std::string& p_name, const proc_pointer& p_pointer, const type_desc& p_type) :
    m_name(p_name),
    m_pointer(p_pointer),
    m_type(p_type)
{ }


edit_table_entry::edit_table_entry(const proc_pointer& p_pointer, const type_desc& p_type) :
    edit_table_entry(std::string(), p_pointer, p_type)
{ }


void edit_table_entry::set_name(const std::string& p_name) {
    m_name = p_name;
}


void edit_table_entry::set_value(const std::string& p_value) {
    (void)p_value;
}


void edit_table_entry::refresh() {
    return;
}


std::ostream& operator<<(std::ostream& p_stream, const edit_table_entry& p_entry) {
    (void)p_entry;
    return p_stream;
}
