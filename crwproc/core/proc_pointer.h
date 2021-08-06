#pragma once

#include <cstdint>
#include <string>

#include "filter_value.h"


struct proc_pointer {
public:
    std::uint64_t       address = 0;
    std::uint64_t       size = 0;
    filter_value::type  type = filter_value::type::invalid;
    std::string         value;
};


using proc_pointer_sequence = std::vector<proc_pointer>;
