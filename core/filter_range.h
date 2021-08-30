/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/

#pragma once

#include <iostream>
#include <string>

#include "value.h"


class filter_range {
private:
    value       m_begin;
    value       m_end;

public:
    filter_range() = default;

    filter_range(const value& p_begin, const value& p_end);

public: /* template filter contract section */
    bool is_valid() const;

    bool is_value_signed() const;

    value::type get_value_type() const;

    std::size_t get_value_size() const;

    template <typename TypeValue>
    bool is_satisfying(TypeValue p_value) const {
        return (p_value >= m_begin.get<TypeValue>()) && (p_value <= m_end.get<TypeValue>());
    }

public: /* class specific section */
    template <typename TypeValue>
    void set_begin_value(TypeValue p_value) {
        m_begin.set(p_value);
    }

    template <typename TypeValue>
    void set_end_value(TypeValue p_value) {
        m_end.set(p_value);
    }

public:
    friend std::ostream& operator<<(std::ostream& p_stream, const filter_range& p_info);
};

