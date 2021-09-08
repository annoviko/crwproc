/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "ut-filter_less.h"


TEST(ut_filter_less, find_value_int8_in_stack) {
    using TypeValue = std::int8_t;
    TypeValue stack_value(-1);
    test_template_filter_less(&stack_value, TypeValue(1));
}


TEST(ut_filter_less, find_value_int16_in_stack) {
    using TypeValue = std::int16_t;
    TypeValue stack_value(-16384);
    test_template_filter_less(&stack_value, TypeValue(1));
}


TEST(ut_filter_less, find_value_int32_in_stack) {
    using TypeValue = std::int32_t;
    TypeValue stack_value(-268435456);
    test_template_filter_less(&stack_value, TypeValue(1));
}


TEST(ut_filter_less, find_value_int64_in_stack) {
    using TypeValue = std::int64_t;
    TypeValue stack_value(-1099511627776);
    test_template_filter_less(&stack_value, TypeValue(1));
}


TEST(ut_filter_less, find_value_uint8_in_stack) {
    using TypeValue = std::uint8_t;
    TypeValue stack_value(120);
    test_template_filter_less(&stack_value, TypeValue(1));
}


TEST(ut_filter_less, find_value_uint16_in_stack) {
    using TypeValue = std::uint16_t;
    TypeValue stack_value(16384);
    test_template_filter_less(&stack_value, TypeValue(1));
}


TEST(ut_filter_less, find_value_uint32_in_stack) {
    using TypeValue = std::uint32_t;
    TypeValue stack_value(268435456);
    test_template_filter_less(&stack_value, TypeValue(1));
}


TEST(ut_filter_less, find_value_uint64_in_stack) {
    using TypeValue = std::uint64_t;
    TypeValue stack_value(1099511627776);
    test_template_filter_less(&stack_value, TypeValue(1));
}


TEST(ut_filter_less, find_value_float_in_stack) {
    using TypeValue = float;
    TypeValue stack_value = float(6.154);
    test_template_filter_less(&stack_value, TypeValue(1.5));
}


TEST(ut_filter_less, find_value_double_in_stack) {
    using TypeValue = double;
    TypeValue stack_value(-5.67);
    test_template_filter_less(&stack_value, TypeValue(1.8));
}