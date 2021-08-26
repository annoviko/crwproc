/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include <gtest/gtest.h>

#include <cstddef>
#include <functional>
#include <string>

#include <windows.h>

#include "core/proc_table.h"
#include "core/proc_reader.h"


std::uint32_t value_uint32_in_data_segment = 149567;


using incrementor = std::function<void(void)>;


static void test_template_find_value(const value::type p_type, const std::size_t p_size, const void* p_pointer, const incrementor& p_func) {
    std::size_t pid = static_cast<std::size_t>(GetCurrentProcessId());
    proc_info info = proc_table().get().at(pid);

    filter_equal filter(value(p_type, p_size, p_pointer));
    auto result = proc_reader(info, filter).read_and_filter();

    ASSERT_FALSE(result.empty());

    const static std::size_t attempts_limit = 10;
    for (std::size_t i = 0; (i < attempts_limit) && (result.size() > 1); i++) {
        p_func();
        filter_equal updated_filter = filter_equal(value(p_type, p_size, p_pointer));
        result = proc_reader(info, updated_filter).read_and_filter(result);
    }

    ASSERT_FALSE(result.empty());

    int index_value = -1;
    for (std::size_t i = 0; i < result.size(); i++) {
        if (result[i].get_address() == (std::uint64_t)p_pointer) {
            index_value = (int)i;
            break;
        }
    }

    ASSERT_NE(-1, index_value);
    ASSERT_EQ((std::uint64_t)p_pointer, result[index_value].get_address());
}


TEST(ut_proc_reader, find_value_int8_in_stack) {
    std::int8_t stack_value = -64;
    test_template_find_value(value::type::integral, sizeof(decltype(stack_value)), &stack_value, [&stack_value]() { stack_value++; });
}


TEST(ut_proc_reader, find_value_int16_in_stack) {
    std::int16_t stack_value = -4096;
    test_template_find_value(value::type::integral, sizeof(decltype(stack_value)), &stack_value, [&stack_value]() { stack_value++; });
}


TEST(ut_proc_reader, find_value_int32_in_stack) {
    std::int32_t stack_value = -95678;
    test_template_find_value(value::type::integral, sizeof(decltype(stack_value)), &stack_value, [&stack_value]() { stack_value++; });
}


TEST(ut_proc_reader, find_value_int64_in_stack) {
    std::int64_t stack_value = -3333333333;
    test_template_find_value(value::type::integral, sizeof(decltype(stack_value)), &stack_value, [&stack_value]() { stack_value++; });
}


TEST(ut_proc_reader, find_value_uint8_in_stack) {
    std::uint8_t stack_value = 128;
    test_template_find_value(value::type::integral, sizeof(decltype(stack_value)), &stack_value, [&stack_value]() { stack_value++; });
}


TEST(ut_proc_reader, find_value_uint16_in_stack) {
    std::uint16_t stack_value = 2048;
    test_template_find_value(value::type::integral, sizeof(decltype(stack_value)), &stack_value, [&stack_value]() { stack_value++; });
}


TEST(ut_proc_reader, find_value_uint32_in_stack) {
    std::uint32_t stack_value = 75632;
    test_template_find_value(value::type::integral, sizeof(decltype(stack_value)), &stack_value, [&stack_value]() { stack_value++; });
}


TEST(ut_proc_reader, find_value_uint64_in_stack) {
    std::uint64_t stack_value = 1000000000;
    test_template_find_value(value::type::integral, sizeof(decltype(stack_value)), &stack_value, [&stack_value]() { stack_value++; });
}


TEST(ut_proc_reader, find_value_float_in_stack) {
    float stack_value = float(3.14);
    test_template_find_value(value::type::integral, sizeof(decltype(stack_value)), &stack_value, [&stack_value]() { stack_value += float(0.1); });
}


TEST(ut_proc_reader, find_value_double_in_stack) {
    double stack_value = 7.1234;
    test_template_find_value(value::type::integral, sizeof(decltype(stack_value)), &stack_value, [&stack_value]() { stack_value += 0.1; });
}


TEST(ut_proc_reader, find_value_uint8_in_heap) {
    std::uint8_t* stack_value = new std::uint8_t(128);
    test_template_find_value(value::type::integral, sizeof(decltype(stack_value)), &stack_value, [&stack_value]() { stack_value++; });
}


TEST(ut_proc_reader, find_value_uint16_in_heap) {
    std::uint16_t* stack_value = new std::uint16_t(2048);
    test_template_find_value(value::type::integral, sizeof(decltype(stack_value)), &stack_value, [&stack_value]() { stack_value++; });
}


TEST(ut_proc_reader, find_value_uint32_in_heap) {
    std::uint32_t* stack_value = new std::uint32_t(75632);
    test_template_find_value(value::type::integral, sizeof(decltype(stack_value)), &stack_value, [&stack_value]() { stack_value++; });
}


TEST(ut_proc_reader, find_value_uint64_in_heap) {
    std::uint64_t* stack_value = new std::uint64_t(1000000000);
    test_template_find_value(value::type::integral, sizeof(decltype(stack_value)), &stack_value, [&stack_value]() { stack_value++; });
}


TEST(ut_proc_reader, find_value_float_in_heap) {
    float* stack_value = new float(3.14f);
    test_template_find_value(value::type::integral, sizeof(decltype(stack_value)), &stack_value, [&stack_value]() { *stack_value += float(0.1); });
}


TEST(ut_proc_reader, find_value_double_in_heap) {
    double* stack_value = new double(3.14);
    test_template_find_value(value::type::integral, sizeof(decltype(stack_value)), &stack_value, [&stack_value]() { *stack_value += 0.1; });
}
