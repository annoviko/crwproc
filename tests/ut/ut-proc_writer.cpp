/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/

#include "ut-proc_writer.h"

#include <cstddef>


TEST(ut_proc_writer, write_int8) {
    std::int8_t variable = -10, expected = -112;
    test_template_write_value(&variable, expected);
    ASSERT_EQ(expected, variable);
}


TEST(ut_proc_writer, write_int16) {
    std::int16_t variable = -2048, expected = -4096;
    test_template_write_value(&variable, expected);
    ASSERT_EQ(expected, variable);
}


TEST(ut_proc_writer, write_int32) {
    std::int32_t variable = -26843545, expected = -64;
    test_template_write_value(&variable, expected);
    ASSERT_EQ(expected, variable);
}


TEST(ut_proc_writer, write_int64) {
    std::int64_t variable = -1152921504606846976, expected = -1122921504606846867;
    test_template_write_value(&variable, expected);
    ASSERT_EQ(expected, variable);
}


TEST(ut_proc_writer, write_uint8) {
    std::uint8_t variable = 10, expected = 112;
    test_template_write_value(&variable, expected);
    ASSERT_EQ(expected, variable);
}


TEST(ut_proc_writer, write_uint16) {
    std::uint16_t variable = 2048, expected = 4096;
    test_template_write_value(&variable, expected);
    ASSERT_EQ(expected, variable);
}


TEST(ut_proc_writer, write_uint32) {
    std::uint32_t variable = 26843545, expected = 64;
    test_template_write_value(&variable, expected);
    ASSERT_EQ(expected, variable);
}


TEST(ut_proc_writer, write_uint64) {
    std::uint64_t variable = 1152921504606846976, expected = 1122921504606846867;
    test_template_write_value(&variable, expected);
    ASSERT_EQ(expected, variable);
}


TEST(ut_proc_writer, write_float) {
    float variable = float(1.1845), expected = float(1678.9457);
    test_template_write_value(&variable, expected);
    ASSERT_EQ(expected, variable);
}


TEST(ut_proc_writer, write_double) {
    double variable = 18.457638, expected = -683.5878;
    test_template_write_value(&variable, expected);
    ASSERT_EQ(expected, variable);
}
