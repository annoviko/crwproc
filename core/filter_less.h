/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once

#include <iostream>
#include <string>

#include "value.h"


class filter_less {
private:
    value       m_blank_value;

public:
    filter_less() = default;

    filter_less(const value& p_value);

public: /* template filter contract section */
    bool is_valid() const;

    bool is_value_signed() const;

    value::type get_value_type() const;

    std::size_t get_value_size() const;

    template <typename TypeValue>
    bool is_satisfying(TypeValue p_current, TypeValue p_previous) const {
        return p_current < p_previous;
    }

public: /* class specific section */
    template <typename TypeValue>
    void set_value(TypeValue p_value) {
        m_blank_value.set(p_value);
    }

public:
    friend std::ostream& operator<<(std::ostream& p_stream, const filter_less& p_info);
};