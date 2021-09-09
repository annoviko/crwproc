/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/

#pragma once


#include <gtest/gtest.h>

#include "core/type_desc.h"


template <typename TypeValue>
void test_template_create_type() {
    type_desc type = type_desc::create<TypeValue>();

    ASSERT_EQ(std::is_signed<TypeValue>::value, type.is_signed());
    ASSERT_EQ(sizeof(TypeValue), type.get_size());

    if constexpr (std::is_integral<TypeValue>::value) {
        ASSERT_EQ(value_type::integral, type.get_type());
    }
    else if constexpr (std::is_same<TypeValue, double>::value) {
        ASSERT_EQ(value_type::doubling, type.get_type());
    }
    else if constexpr (std::is_same<TypeValue, float>::value) {
        ASSERT_EQ(value_type::floating, type.get_type());
    }
}