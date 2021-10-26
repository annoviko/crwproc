/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once

#include <iostream>
#include <string>

#include "filter_base.h"


class filter_none : public filter_base {
public:
    filter_none() = default;

public:
    bool is_valid() const override;

    const type_desc& get_type() const override;

    type_desc& get_type() override;

    template <typename TypeValue>
    static bool is_satisfying(TypeValue) {
        return true;
    }

public:
    friend std::ostream& operator<<(std::ostream& p_stream, const filter_none& p_filter);
};