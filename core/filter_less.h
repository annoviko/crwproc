/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once

#include <iostream>

#include "filter_base.h"


class filter_less : public filter_base {
public:
    filter_less() = default;

    filter_less(const type_desc& p_type);

public: /* template contract */
    template <typename TypeValue>
    bool is_satisfying(TypeValue p_current, TypeValue p_previous) const {
        return p_current < p_previous;
    }

public:
    friend std::ostream& operator<<(std::ostream& p_stream, const filter_less& p_info);
};