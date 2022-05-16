/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once


#include <ostream>
#include <string>

#include "core/proc_info.h"
#include "core/proc_pointer.h"
#include "core/type_desc.h"


class edit_table_entry {
public:
    static constexpr char INVALID_VALUE[] = "########";

    static constexpr std::int64_t DEFAULT_MULTIPLIER = 1;

private:
    std::string     m_name;
    std::int64_t    m_multiplier = DEFAULT_MULTIPLIER;
    proc_pointer    m_pointer;
    type_desc       m_type;

public:
    edit_table_entry() = default;

    edit_table_entry(const std::string& p_name, const proc_pointer& p_pointer, const type_desc& p_type);

    edit_table_entry(const proc_pointer& p_pointer, const type_desc& p_type);

public:
    const std::string& get_name() const;

    const std::int64_t get_multiplier() const;

    const proc_pointer& get_pointer() const;

    const std::string get_value() const;

    const bool is_valid() const;

    const type_desc& get_type() const;

    void set_name(const std::string& p_name);

    void set_multiplier(const std::int64_t p_multiplier);

    bool set_value(const std::string& p_value, const proc_info& p_info);

    void refresh(const proc_info& p_info);

public:
    bool operator==(const edit_table_entry& p_entry) const;

    friend std::ostream& operator<<(std::ostream& p_stream, const edit_table_entry& p_entry);
};