/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "ut-value.h"

#include <gtest/gtest.h>

#include "core/value.h"



TEST(ut_value, create_int8_from_buffer) {
    test_template_value_from_buffer(std::int8_t(-65));
}


TEST(ut_value, create_int16_from_buffer) {
    test_template_value_from_buffer(std::int16_t(-512));
}


TEST(ut_value, create_int32_from_buffer) {
    test_template_value_from_buffer(std::int32_t(-33554432));
}


TEST(ut_value, create_int64_from_buffer) {
    test_template_value_from_buffer(std::int64_t(-68719476736));
}


TEST(ut_value, create_uint8_from_buffer) {
    test_template_value_from_buffer(std::uint8_t(16));
}


TEST(ut_value, create_uint16_from_buffer) {
    test_template_value_from_buffer(std::uint16_t(512));
}


TEST(ut_value, create_uint32_from_buffer) {
    test_template_value_from_buffer(std::uint32_t(4000000));
}


TEST(ut_value, create_uint64_from_buffer) {
    test_template_value_from_buffer(std::uint64_t(68719476736));
}


TEST(ut_value, create_float_from_buffer) {
    test_template_value_from_buffer(float(3.14));
}


TEST(ut_value, create_double_from_buffer) {
    test_template_value_from_buffer(double(6.022));
}
