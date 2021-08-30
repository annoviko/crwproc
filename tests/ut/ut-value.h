/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once


#include <gtest/gtest.h>

#include "core/value.h"


template <typename TypeValue>
typename std::enable_if<std::is_fundamental<TypeValue>::value, void>::type
test_template_value_from_buffer(TypeValue p_value) {
    value val = value::create(p_value);

    ASSERT_TRUE(val.is_valid());
    ASSERT_EQ(sizeof(TypeValue), val.get_size());
    ASSERT_EQ(value::get_type<TypeValue>(), val.get_type());
    ASSERT_EQ(p_value, val.get<TypeValue>());
    ASSERT_EQ(std::to_string(p_value), val.get<std::string>());
}


template <typename TypeValue>
typename std::enable_if<std::is_fundamental<TypeValue>::value, void>::type
test_template_value_from_string(const std::string& p_str_value, const TypeValue p_value) {
    value val;

    if constexpr (std::is_signed<TypeValue>::value) {
        val = value(value::get_type<TypeValue>(), sizeof(TypeValue), true, p_str_value);
    }
    else if constexpr (std::is_unsigned<TypeValue>::value) {
        val = value(value::get_type<TypeValue>(), sizeof(TypeValue), false, p_str_value);
    }
    else {
        static_assert(false);
    }

    ASSERT_TRUE(val.is_valid());
    ASSERT_EQ(sizeof(TypeValue), val.get_size());
    ASSERT_EQ(value::get_type<TypeValue>(), val.get_type());
    ASSERT_EQ(p_value, val.get<TypeValue>());
    ASSERT_EQ(p_str_value, val.get<std::string>());
}