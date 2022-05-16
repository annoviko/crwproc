/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once

#include <iostream>

#include "filter_base.h"


class filter_change : public filter_base {
public:
    filter_change() = default;

    explicit filter_change(const type_desc& p_type);

public: /* template contract */
    template <typename TypeValue>
    static bool is_satisfying(TypeValue p_current, TypeValue p_previous) {
        return p_current != p_previous;
    }

public:
    friend std::ostream& operator<<(std::ostream& p_stream, const filter_change& p_info);
};