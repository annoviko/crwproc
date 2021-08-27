/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "ut-proc_reader.h"


std::uint8_t  value_uint8_in_data_segment = 128;
std::uint16_t value_uint16_in_data_segment = 40000;
std::uint32_t value_uint32_in_data_segment = 149567;
std::uint64_t value_uint64_in_data_segment = 2000000000;


TEST(ut_proc_reader, find_value_int8_in_data_segment) {
    test_template_find_value(&value_uint8_in_data_segment, []() { value_uint8_in_data_segment++; });
}


TEST(ut_proc_reader, find_value_int16_in_data_segment) {
    test_template_find_value(&value_uint16_in_data_segment, []() { value_uint16_in_data_segment++; });
}


TEST(ut_proc_reader, find_value_int32_in_data_segment) {
    test_template_find_value(&value_uint32_in_data_segment, []() { value_uint32_in_data_segment++; });
}


TEST(ut_proc_reader, find_value_int64_in_data_segment) {
    test_template_find_value(&value_uint64_in_data_segment, []() { value_uint64_in_data_segment++; });
}


TEST(ut_proc_reader, find_value_int8_in_stack) {
    std::int8_t stack_value = -64;
    test_template_find_value(&stack_value, [&stack_value]() { stack_value++; });
}


TEST(ut_proc_reader, find_value_int16_in_stack) {
    std::int16_t stack_value = -4096;
    test_template_find_value(&stack_value, [&stack_value]() { stack_value++; });
}


TEST(ut_proc_reader, find_value_int32_in_stack) {
    std::int32_t stack_value = -95678;
    test_template_find_value(&stack_value, [&stack_value]() { stack_value++; });
}


TEST(ut_proc_reader, find_value_int64_in_stack) {
    std::int64_t stack_value = -3333333333;
    test_template_find_value(&stack_value, [&stack_value]() { stack_value++; });
}


TEST(ut_proc_reader, find_value_uint8_in_stack) {
    std::uint8_t stack_value = 128;
    test_template_find_value(&stack_value, [&stack_value]() { stack_value++; });
}


TEST(ut_proc_reader, find_value_uint16_in_stack) {
    std::uint16_t stack_value = 2048;
    test_template_find_value(&stack_value, [&stack_value]() { stack_value++; });
}


TEST(ut_proc_reader, find_value_uint32_in_stack) {
    std::uint32_t stack_value = 75632;
    test_template_find_value(&stack_value, [&stack_value]() { stack_value++; });
}


TEST(ut_proc_reader, find_value_uint64_in_stack) {
    std::uint64_t stack_value = 1000000000;
    test_template_find_value(&stack_value, [&stack_value]() { stack_value++; });
}


TEST(ut_proc_reader, find_value_float_in_stack) {
    float stack_value = float(3.14);
    test_template_find_value(&stack_value, [&stack_value]() { stack_value += float(0.1); });
}


TEST(ut_proc_reader, find_value_double_in_stack) {
    double stack_value = 7.1234;
    test_template_find_value(&stack_value, [&stack_value]() { stack_value += 0.1; });
}


TEST(ut_proc_reader, find_value_uint8_in_heap) {
    std::shared_ptr<std::uint8_t> stack_value = std::make_shared<std::uint8_t>(64);
    test_template_find_value(stack_value.get(), [stack_value]() { (*stack_value)++; });
}


TEST(ut_proc_reader, find_value_uint16_in_heap) {
    std::shared_ptr<std::uint16_t> stack_value = std::make_shared<std::uint16_t>(2048);
    test_template_find_value(stack_value.get(), [stack_value]() { (*stack_value)++; });
}


TEST(ut_proc_reader, find_value_uint32_in_heap) {
    std::shared_ptr<std::uint32_t> stack_value = std::make_shared<std::uint32_t>(75632);
    test_template_find_value(stack_value.get(), [stack_value]() { (*stack_value)++; });
}


TEST(ut_proc_reader, find_value_uint64_in_heap) {
    std::shared_ptr<std::uint64_t> stack_value = std::make_shared<std::uint64_t>(1000000000);
    test_template_find_value(stack_value.get(), [stack_value]() { (*stack_value)++; });
}


TEST(ut_proc_reader, find_value_float_in_heap) {
    std::shared_ptr<float> stack_value = std::make_shared<float>(3.14f);
    test_template_find_value(stack_value.get(), [stack_value]() { (*stack_value) += float(0.1); });
}


TEST(ut_proc_reader, find_value_double_in_heap) {
    std::shared_ptr<double> stack_value = std::make_shared<double>(7.105);
    test_template_find_value(stack_value.get(), [stack_value]() { (*stack_value) += 0.1; });
}
