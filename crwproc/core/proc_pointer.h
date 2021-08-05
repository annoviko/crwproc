#pragma once

#include <cstdint>

#include "filter_value.h"


struct proc_pointer {
public:
    std::uint64_t       address = 0;
    std::uint64_t       size = 0;
    filter_value::type  type = filter_value::type::invalid;
};


using proc_pointer_sequence = std::vector<proc_pointer>;
