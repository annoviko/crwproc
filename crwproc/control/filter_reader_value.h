#pragma once

#include <string>

#include "core/filter_value.h"


class filter_reader_value {
public:
    static std::string read(const filter_value& p_filter);
};