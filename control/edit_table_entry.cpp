/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "edit_table_entry.h"

#include "core/proc_reader.h"
#include "core/proc_writer.h"


edit_table_entry::edit_table_entry(const std::string& p_name, const proc_pointer& p_pointer, const type_desc& p_type) :
    m_name(p_name),
    m_pointer(p_pointer),
    m_type(p_type)
{ }


edit_table_entry::edit_table_entry(const proc_pointer& p_pointer, const type_desc& p_type) :
    edit_table_entry(std::string(), p_pointer, p_type)
{ }


const std::string& edit_table_entry::get_name() const {
    return m_name;
}


const std::int64_t edit_table_entry::get_multiplier() const {
    return m_multiplier;
}


const proc_pointer& edit_table_entry::get_pointer() const {
    return m_pointer;
}


const std::string edit_table_entry::get_value() const {
    return is_valid() ? get_pointer().get_value().to_string(get_type(), m_multiplier) : INVALID_VALUE;
}


const bool edit_table_entry::is_valid() const {
    return m_pointer.is_valid();
}


const type_desc& edit_table_entry::get_type() const {
    return m_type;
}


void edit_table_entry::set_name(const std::string& p_name) {
    m_name = p_name;
}


void edit_table_entry::set_multiplier(const std::int64_t p_multiplier) {
    m_multiplier = p_multiplier;
}


bool edit_table_entry::set_value(const std::string& p_value, const proc_info& p_info) {
    proc_pointer next_value = m_pointer;
    next_value.get_value().set(p_value, m_type, m_multiplier);

    proc_writer writer(p_info);
    if (writer.write(next_value, m_type)) {
        m_pointer = next_value;
        return true;
    }

    return false;
}


void edit_table_entry::refresh(const proc_info& p_info) {
    proc_reader(p_info).refresh(m_type, m_pointer);
}


bool edit_table_entry::operator==(const edit_table_entry& p_entry) const {
    return (m_name == p_entry.m_name) &&
        (m_pointer == p_entry.m_pointer) &&
        (m_type == p_entry.m_type);
}


std::ostream& operator<<(std::ostream& p_stream, const edit_table_entry& p_entry) {
    (void)p_entry;
    return p_stream;
}
