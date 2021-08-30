/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once

#include <cstdint>
#include <string>

#include "value.h"


struct proc_pointer {
private:
    std::uint64_t   m_address = 0;
    value           m_value;

public:
    proc_pointer() = default;

    proc_pointer(const std::uint64_t p_address, const value& p_value);

public:
    bool is_valid() const;

    std::uint64_t get_address() const;

    value& get_value();

    const value& get_value() const;
};


using proc_pointer_sequence = std::vector<proc_pointer>;
