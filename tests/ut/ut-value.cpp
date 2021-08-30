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


TEST(ut_value, create_int8_from_string) {
    test_template_value_from_string("0", std::int8_t(0));
    test_template_value_from_string("-100", std::int8_t(-100));
    test_template_value_from_string("120", std::int8_t(120));
}


TEST(ut_value, create_int16_from_string) {
    test_template_value_from_string("32760", std::int16_t(32760));
    test_template_value_from_string("-32760", std::int16_t(-32760));
}


TEST(ut_value, create_int32_from_string) {
    test_template_value_from_string("536870912", std::int32_t(536870912));
    test_template_value_from_string("-536870912", std::int32_t(-536870912));
}


TEST(ut_value, create_int64_from_string) {
    test_template_value_from_string("72057594037927936", std::int64_t(72057594037927936));
    test_template_value_from_string("-72057594037927936", std::int64_t(-72057594037927936));
}


TEST(ut_value, create_uint8_from_string) {
    test_template_value_from_string("0", std::uint8_t(0));
    test_template_value_from_string("127", std::uint8_t(127));
    test_template_value_from_string("254", std::uint8_t(254));
}


TEST(ut_value, create_uint16_from_string) {
    test_template_value_from_string("65535", std::uint16_t(65535));
}


TEST(ut_value, create_uint32_from_string) {
    test_template_value_from_string("2147483648", std::uint32_t(2147483648));
}


TEST(ut_value, create_uint64_from_string) {
    test_template_value_from_string("68719476736", std::uint64_t(68719476736));
}