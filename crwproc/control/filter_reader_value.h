#pragma once

#include <string>

#include "core/filter_equal.h"


class filter_reader_value {
public:
    static std::string read(const filter_equal& p_filter);
};