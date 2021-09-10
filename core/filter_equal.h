/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once

#include <iostream>
#include <string>

#include "filter_base.h"
#include "memory_value.h"


class filter_equal : public filter_base {
private:
    memory_value       m_value = memory_value{ };

public:
    filter_equal() = default;

    filter_equal(const type_desc& p_type);

    filter_equal(const memory_value& p_value, const type_desc& p_type);

public: /* template contract */
    template <typename TypeValue>
    bool is_satisfying(TypeValue p_value) const {
        return m_value.get<TypeValue>() == p_value;
    }

public: /* class specific section */
    template <typename TypeValue>
    void set_value(TypeValue p_value) {
        m_value.set(p_value);
    }

    template <>
    void set_value<std::string>(std::string p_value) {
        m_value.set(p_value, m_type);
    }

public:
    friend std::ostream& operator<<(std::ostream& p_stream, const filter_equal& p_filter);
};
