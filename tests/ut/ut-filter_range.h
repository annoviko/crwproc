/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/

#pragma once


#include "ut-proc_reader.h"

#include "core/filter_range.h"


template <typename TypeValue>
void test_template_filter_range(TypeValue* p_value, TypeValue p_value_delta, TypeValue p_range_delta) {
    auto filter_updater = [p_value, p_range_delta]() {
        type_desc type = type_desc::create<TypeValue>();
        filter_range filter(type);
        filter.set_begin_value(TypeValue(*p_value - p_range_delta));
        filter.set_end_value(TypeValue(*p_value + p_range_delta));
        return filter;
    };

    test_template_find_value<filter_range>(p_value, [p_value, p_value_delta]() { *(p_value) += p_value_delta; }, filter_updater);
}
