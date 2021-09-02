/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once

#include <cstdint>
#include <limits>
#include <string>

#include "value.h"


struct proc_pointer {
public:
    static constexpr std::uint64_t INVALID_ADDRESS = 0;

private:
    std::uint64_t   m_address = INVALID_ADDRESS;
    std::uint64_t   m_base_address = INVALID_ADDRESS;
    value           m_value;

public:
    proc_pointer() = default;

    proc_pointer(const std::uint64_t p_address, const value& p_value);

    proc_pointer(const std::uint64_t p_address, const std::uint64_t p_base_address, const value& p_value);

public:
    bool is_valid() const;

    std::uint64_t get_address() const;

    std::uint64_t get_base_address() const;

    value& get_value();

    const value& get_value() const;
};


using proc_pointer_sequence = std::vector<proc_pointer>;
