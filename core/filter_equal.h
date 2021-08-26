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

    value& get_value();

    const value& get_value() const;

    template <typename TypeValue>
    bool is_satisfying(TypeValue p_value) const {
        return m_value.get<TypeValue>() == p_value;
    }

public:
    friend std::ostream& operator<<(std::ostream& p_stream, const filter_equal& p_info);
};
