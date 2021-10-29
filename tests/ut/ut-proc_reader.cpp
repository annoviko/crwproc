/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "ut-proc_reader.h"

#include <filesystem>


struct value_in_struct {
public:
    std::uint8_t uint8 = 120;
    std::uint16_t uint16 = 16384;
    std::uint32_t uint32 = 1073741824;
    std::uint64_t uint64 = 1152921504606846976;

    std::int8_t int8 = -60;
    std::int16_t int16 = -8192;
    std::int32_t int32 = -536870912;
    std::int64_t int64 = -576460752303423488;

    float float_ = float(3.14);
    double double_ = -3.14;

public:
    void reset() {
        uint8 = 120;
        uint16 = 16384;
        uint32 = 1073741824;
        uint64 = 1152921504606846976;

        int8 = -60;
        int16 = -8192;
        int32 = -536870912;
        int64 = -576460752303423488;

        float_ = float(3.14);
        double_ = -3.14;
    }

    void increment() {
        uint8++;
        uint16++;
        uint32++;
        uint64++;

        int8++;
        int16++;
        int32++;
        int64++;

        float_ += float(0.1);
        double_ += 0.1;
    }

    void decrement() {
        uint8--;
        uint16--;
        uint32--;
        uint64--;

        int8--;
        int16--;
        int32--;
        int64--;

        float_ -= float(0.1);
        double_ -= 0.1;
    }
};


std::uint8_t  value_uint8_in_data_segment = 128;
std::uint16_t value_uint16_in_data_segment = 40000;
std::uint32_t value_uint32_in_data_segment = 149567;
std::uint64_t value_uint64_in_data_segment = 2000000000;

value_in_struct value_in_struct_in_data_segment;



TEST(ut_proc_reader, find_value_int8_in_data_segment) {
    test_template_find_value<filter_equal>(&value_uint8_in_data_segment, []() { value_uint8_in_data_segment++; });
}


TEST(ut_proc_reader, find_value_int16_in_data_segment) {
    test_template_find_value<filter_equal>(&value_uint16_in_data_segment, []() { value_uint16_in_data_segment++; });
}


TEST(ut_proc_reader, find_value_int32_in_data_segment) {
    test_template_find_value<filter_equal>(&value_uint32_in_data_segment, []() { value_uint32_in_data_segment++; });
}


TEST(ut_proc_reader, find_value_int64_in_data_segment) {
    test_template_find_value<filter_equal>(&value_uint64_in_data_segment, []() { value_uint64_in_data_segment++; });
}


TEST(ut_proc_reader, find_value_uint8_struct_in_data_segment) {
    value_in_struct_in_data_segment.reset();
    test_template_find_value<filter_equal>(&value_in_struct_in_data_segment.uint8, []() { value_in_struct_in_data_segment.increment(); });
}


TEST(ut_proc_reader, find_value_uint16_struct_in_data_segment) {
    value_in_struct_in_data_segment.reset();
    test_template_find_value<filter_equal>(&value_in_struct_in_data_segment.uint16, []() { value_in_struct_in_data_segment.increment(); });
}


TEST(ut_proc_reader, find_value_uint32_struct_in_data_segment) {
    value_in_struct_in_data_segment.reset();
    test_template_find_value<filter_equal>(&value_in_struct_in_data_segment.uint32, []() { value_in_struct_in_data_segment.increment(); });
}


TEST(ut_proc_reader, find_value_uint64_struct_in_data_segment) {
    value_in_struct_in_data_segment.reset();
    test_template_find_value<filter_equal>(&value_in_struct_in_data_segment.uint64, []() { value_in_struct_in_data_segment.increment(); });
}


TEST(ut_proc_reader, find_value_int8_struct_in_data_segment) {
    value_in_struct_in_data_segment.reset();
    test_template_find_value<filter_equal>(&value_in_struct_in_data_segment.int8, []() { value_in_struct_in_data_segment.increment(); });
}


TEST(ut_proc_reader, find_value_int16_struct_in_data_segment) {
    value_in_struct_in_data_segment.reset();
    test_template_find_value<filter_equal>(&value_in_struct_in_data_segment.int16, []() { value_in_struct_in_data_segment.increment(); });
}


TEST(ut_proc_reader, find_value_int32_struct_in_data_segment) {
    value_in_struct_in_data_segment.reset();
    test_template_find_value<filter_equal>(&value_in_struct_in_data_segment.int32, []() { value_in_struct_in_data_segment.increment(); });
}


TEST(ut_proc_reader, find_value_int64_struct_in_data_segment) {
    value_in_struct_in_data_segment.reset();
    test_template_find_value<filter_equal>(&value_in_struct_in_data_segment.int64, []() { value_in_struct_in_data_segment.increment(); });
}


TEST(ut_proc_reader, find_value_float_struct_in_data_segment) {
    value_in_struct_in_data_segment.reset();
    test_template_find_value<filter_equal>(&value_in_struct_in_data_segment.float_, []() { value_in_struct_in_data_segment.increment(); });
}


TEST(ut_proc_reader, find_value_double_struct_in_data_segment) {
    value_in_struct_in_data_segment.reset();
    test_template_find_value<filter_equal>(&value_in_struct_in_data_segment.double_, []() { value_in_struct_in_data_segment.increment(); });
}


TEST(ut_proc_reader, find_value_int8_in_stack) {
    std::int8_t stack_value = -64;
    test_template_find_value<filter_equal>(&stack_value, [&stack_value]() { stack_value++; });
}


TEST(ut_proc_reader, find_value_int16_in_stack) {
    std::int16_t stack_value = -4096;
    test_template_find_value<filter_equal>(&stack_value, [&stack_value]() { stack_value++; });
}


TEST(ut_proc_reader, find_value_int32_in_stack) {
    std::int32_t stack_value = -95678;
    test_template_find_value<filter_equal>(&stack_value, [&stack_value]() { stack_value++; });
}


TEST(ut_proc_reader, find_value_int64_in_stack) {
    std::int64_t stack_value = -3333333333;
    test_template_find_value<filter_equal>(&stack_value, [&stack_value]() { stack_value++; });
}


TEST(ut_proc_reader, find_value_uint8_in_stack) {
    std::uint8_t stack_value = 128;
    test_template_find_value<filter_equal>(&stack_value, [&stack_value]() { stack_value++; });
}


TEST(ut_proc_reader, find_value_uint16_in_stack) {
    std::uint16_t stack_value = 2048;
    test_template_find_value<filter_equal>(&stack_value, [&stack_value]() { stack_value++; });
}


TEST(ut_proc_reader, find_value_uint32_in_stack) {
    std::uint32_t stack_value = 75632;
    test_template_find_value<filter_equal>(&stack_value, [&stack_value]() { stack_value++; });
}


TEST(ut_proc_reader, find_value_uint64_in_stack) {
    std::uint64_t stack_value = 1000000000;
    test_template_find_value<filter_equal>(&stack_value, [&stack_value]() { stack_value++; });
}


TEST(ut_proc_reader, find_value_float_in_stack) {
    float stack_value = float(3.14);
    test_template_find_value<filter_equal>(&stack_value, [&stack_value]() { stack_value += float(0.1); });
}


TEST(ut_proc_reader, find_value_double_in_stack) {
    double stack_value = 7.1234;
    test_template_find_value<filter_equal>(&stack_value, [&stack_value]() { stack_value += 0.1; });
}


TEST(ut_proc_reader, find_value_uint8_in_heap) {
    std::shared_ptr<std::uint8_t> stack_value = std::make_shared<std::uint8_t>(static_cast<std::uint8_t>(64));
    test_template_find_value<filter_equal>(stack_value.get(), [stack_value]() { (*stack_value)++; });
}


TEST(ut_proc_reader, find_value_uint16_in_heap) {
    std::shared_ptr<std::uint16_t> stack_value = std::make_shared<std::uint16_t>(static_cast<std::uint16_t>(2048));
    test_template_find_value<filter_equal>(stack_value.get(), [stack_value]() { (*stack_value)++; });
}


TEST(ut_proc_reader, find_value_uint32_in_heap) {
    std::shared_ptr<std::uint32_t> stack_value = std::make_shared<std::uint32_t>(static_cast<std::uint32_t>(75632));
    test_template_find_value<filter_equal>(stack_value.get(), [stack_value]() { (*stack_value)++; });
}


TEST(ut_proc_reader, find_value_uint64_in_heap) {
    std::shared_ptr<std::uint64_t> stack_value = std::make_shared<std::uint64_t>(static_cast<std::uint64_t>(1000000000));
    test_template_find_value<filter_equal>(stack_value.get(), [stack_value]() { (*stack_value)++; });
}


TEST(ut_proc_reader, find_value_float_in_heap) {
    std::shared_ptr<float> heap_value = std::make_shared<float>(3.14f);
    test_template_find_value<filter_equal>(heap_value.get(), [heap_value]() { (*heap_value) += float(0.1); });
}


TEST(ut_proc_reader, find_value_double_in_heap) {
    std::shared_ptr<double> heap_value = std::make_shared<double>(7.105);
    test_template_find_value<filter_equal>(heap_value.get(), [heap_value]() { (*heap_value) += 0.1; });
}


TEST(ut_proc_reader, find_value_uint8_struct_in_heap) {
    std::shared_ptr<value_in_struct> heap_value = std::make_shared<value_in_struct>();
    test_template_find_value<filter_equal>(&(heap_value->uint8), [heap_value]() { heap_value->increment(); });
}


TEST(ut_proc_reader, find_value_uint16_struct_in_heap) {
    std::shared_ptr<value_in_struct> heap_value = std::make_shared<value_in_struct>();
    test_template_find_value<filter_equal>(&(heap_value->uint16), [heap_value]() { heap_value->increment(); });
}


TEST(ut_proc_reader, find_value_uint32_struct_in_heap) {
    std::shared_ptr<value_in_struct> heap_value = std::make_shared<value_in_struct>();
    test_template_find_value<filter_equal>(&(heap_value->uint32), [heap_value]() { heap_value->increment(); });
}

TEST(ut_proc_reader, find_value_uint64_struct_in_heap) {
    std::shared_ptr<value_in_struct> heap_value = std::make_shared<value_in_struct>();
    test_template_find_value<filter_equal>(&(heap_value->uint64), [heap_value]() { heap_value->increment(); });
}


TEST(ut_proc_reader, find_value_int8_struct_in_heap) {
    std::shared_ptr<value_in_struct> heap_value = std::make_shared<value_in_struct>();
    test_template_find_value<filter_equal>(&(heap_value->int8), [heap_value]() { heap_value->increment(); });
}


TEST(ut_proc_reader, find_value_int16_struct_in_heap) {
    std::shared_ptr<value_in_struct> heap_value = std::make_shared<value_in_struct>();
    test_template_find_value<filter_equal>(&(heap_value->int16), [heap_value]() { heap_value->increment(); });
}


TEST(ut_proc_reader, find_value_int32_struct_in_heap) {
    std::shared_ptr<value_in_struct> heap_value = std::make_shared<value_in_struct>();
    test_template_find_value<filter_equal>(&(heap_value->int32), [heap_value]() { heap_value->increment(); });
}

TEST(ut_proc_reader, find_value_int64_struct_in_heap) {
    std::shared_ptr<value_in_struct> heap_value = std::make_shared<value_in_struct>();
    test_template_find_value<filter_equal>(&(heap_value->int64), [heap_value]() { heap_value->increment(); });
}


TEST(ut_proc_reader, find_value_float_struct_in_heap) {
    std::shared_ptr<value_in_struct> heap_value = std::make_shared<value_in_struct>();
    test_template_find_value<filter_equal>(&(heap_value->float_), [heap_value]() { heap_value->increment(); });
}

TEST(ut_proc_reader, find_value_double_struct_in_heap) {
    std::shared_ptr<value_in_struct> heap_value = std::make_shared<value_in_struct>();
    test_template_find_value<filter_equal>(&(heap_value->double_), [heap_value]() { heap_value->increment(); });
}


TEST(ut_proc_reader, find_value_uint8_memory_in_heap) {
    std::shared_ptr<std::uint8_t[]> heap(new std::uint8_t[32768]);
    std::int8_t* heap_value = (std::int8_t*) heap.get() + 16384;
    test_template_find_value<filter_equal>(heap_value, [heap_value]() { (*heap_value)++; });
}


TEST(ut_proc_reader, find_value_uint16_memory_in_heap) {
    std::shared_ptr<std::uint8_t[]> heap(new std::uint8_t[32768]);
    std::uint16_t* heap_value = (std::uint16_t*) ((std::uint8_t*)heap.get() + 16384);
    test_template_find_value<filter_equal>(heap_value, [heap_value]() { (*heap_value)++; });
}


TEST(ut_proc_reader, find_value_uint32_memory_in_heap) {
    std::shared_ptr<std::uint8_t[]> heap(new std::uint8_t[32768]);
    std::uint32_t* heap_value = (std::uint32_t*)((std::uint8_t*)heap.get() + 16384);
    test_template_find_value<filter_equal>(heap_value, [heap_value]() { (*heap_value)++; });
}


TEST(ut_proc_reader, find_value_uin64_memory_in_heap) {
    std::shared_ptr<std::uint8_t[]> heap(new std::uint8_t[32768]);
    std::uint64_t* heap_value = (std::uint64_t*)((std::uint8_t*)heap.get() + 16384);
    test_template_find_value<filter_equal>(heap_value, [heap_value]() { (*heap_value)++; });
}


TEST(ut_proc_reader, find_value_uin64_memory_in_begin_heap) {
    std::shared_ptr<std::uint8_t[]> heap(new std::uint8_t[32768]);
    std::uint64_t* heap_value = (std::uint64_t*)((std::uint8_t*)heap.get());
    test_template_find_value<filter_equal>(heap_value, [heap_value]() { (*heap_value)++; });
}


TEST(ut_proc_reader, find_value_uin64_memory_in_end_heap) {
    std::shared_ptr<std::uint8_t[]> heap(new std::uint8_t[32768]);
    std::uint64_t* heap_value = (std::uint64_t*)((std::uint8_t*)heap.get() + 32768 - sizeof(std::uint64_t));
    test_template_find_value<filter_equal>(heap_value, [heap_value]() { (*heap_value)++; });
}

