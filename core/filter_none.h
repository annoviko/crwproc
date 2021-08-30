/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once

#include <iostream>
#include <string>

#include "value.h"


class filter_none {
public:
    filter_none() = default;

public: /* template filter contract section */
    bool is_valid() const;

    bool is_value_signed() const;

    value::type get_value_type() const;

    std::size_t get_value_size() const;

    template <typename TypeValue>
    bool is_satisfying(TypeValue p_value) const {
        return true;
    }

public:
    friend std::ostream& operator<<(std::ostream& p_stream, const filter_none& p_info);
};