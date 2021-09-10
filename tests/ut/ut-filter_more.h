/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once


#include "ut-proc_reader.h"

#include "core/filter_more.h"


template <typename TypeValue>
void test_template_filter_more(TypeValue* p_value, TypeValue p_value_delta) {
    auto filter_updater = [p_value]() {
        type_desc type = type_desc::create<TypeValue>();
        filter_more filter(type);
        return filter;
    };

    test_template_find_value<filter_more>(p_value, [p_value, p_value_delta]() { *(p_value) += p_value_delta; }, filter_updater);
}
