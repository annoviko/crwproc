/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/

#pragma once

#include <iostream>
#include <string>

#include "filter_base.h"
#include "memory_value.h"


class filter_range : public filter_base {
private:
    memory_value       m_begin;
    memory_value       m_end;

public:
    filter_range() = default;

    explicit filter_range(const type_desc& p_type);

    filter_range(const memory_value& p_begin, const memory_value& p_end, const type_desc& p_type);

public: /* template filter contract section */
    template <typename TypeValue>
    bool is_satisfying(TypeValue p_value) const {
        return (p_value >= m_begin.get<TypeValue>()) && (p_value <= m_end.get<TypeValue>());
    }

public: /* class specific section */
    template <typename TypeValue>
    void set_begin_value(TypeValue p_value) {
        m_begin.set(p_value);
    }

    template <>
    void set_begin_value<std::string>(std::string p_value) {
        m_begin.set(p_value, m_type);
    }

    template <typename TypeValue>
    void set_end_value(TypeValue p_value) {
        m_end.set(p_value);
    }

    template <>
    void set_end_value<std::string>(std::string p_value) {
        m_end.set(p_value, m_type);
    }

public:
    friend std::ostream& operator<<(std::ostream& p_stream, const filter_range& p_filter);
};

