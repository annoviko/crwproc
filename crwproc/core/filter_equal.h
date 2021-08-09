#pragma once

#include <iostream>
#include <string>

#include "value.h"


class filter_equal {
private:
    value       m_value;

public:
    filter_equal() = default;

    filter_equal(const value& p_value);

public:
    bool is_valid() const;

    bool is_satisfying(const std::string& p_value) const;

    value& get_value();

    const value& get_value() const;

public:
    friend std::ostream& operator<<(std::ostream& p_stream, const filter_equal& p_info);
};
