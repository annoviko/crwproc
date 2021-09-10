/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "ut-filter_range.h"


TEST(ut_filter_range, find_value_int8_in_stack) {
    std::int8_t stack_value = -120;
    test_template_filter_range(&stack_value, std::int8_t(10), std::int8_t(5));
}


TEST(ut_filter_range, find_value_int16_in_stack) {
    std::int16_t stack_value = -1024;
    test_template_filter_range(&stack_value, std::int16_t(10), std::int16_t(5));
}


TEST(ut_filter_range, find_value_int32_in_stack) {
    std::int32_t stack_value = -2048;
    test_template_filter_range(&stack_value, std::int32_t(10), std::int32_t(5));
}


TEST(ut_filter_range, find_value_int64_in_stack) {
    std::int64_t stack_value = -1048576;
    test_template_filter_range(&stack_value, std::int64_t(10), std::int64_t(5));
}


TEST(ut_filter_range, find_value_uint8_in_stack) {
    std::uint8_t stack_value = 32;
    test_template_filter_range(&stack_value, std::uint8_t(10), std::uint8_t(5));
}


TEST(ut_filter_range, find_value_uint16_in_stack) {
    std::uint16_t stack_value = 1024;
    test_template_filter_range(&stack_value, std::uint16_t(10), std::uint16_t(5));
}


TEST(ut_filter_range, find_value_uint32_in_stack) {
    std::uint32_t stack_value = 2048;
    test_template_filter_range(&stack_value, std::uint32_t(10), std::uint32_t(5));
}


TEST(ut_filter_range, find_value_uint64_in_stack) {
    std::uint64_t stack_value = 1048576;
    test_template_filter_range(&stack_value, std::uint64_t(10), std::uint64_t(5));
}


TEST(ut_filter_range, find_value_float_in_stack) {
    float stack_value = float(3.14);
    test_template_filter_range(&stack_value, float(0.5), float(0.5));
}


TEST(ut_filter_range, find_value_double_in_stack) {
    double stack_value = -3.14;
    test_template_filter_range(&stack_value, 0.5, 0.5);
}


TEST(ut_filter_range, find_value_int64_in_stack_decrement) {
    std::int64_t stack_value = -1048576;
    test_template_filter_range(&stack_value, std::int64_t(-10), std::int64_t(5));
}


TEST(ut_filter_range, find_value_int64_in_stack_range_2000) {
    std::int64_t stack_value = -1048576;
    test_template_filter_range(&stack_value, std::int64_t(500), std::int64_t(2000));
}
