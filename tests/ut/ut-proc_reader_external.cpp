/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "ut-proc_reader_external.h"


TEST_F(ut_proc_reader_external, find_value_int8_in_global) {
    test_template_external_find_value<std::int8_t>("global");
}


TEST_F(ut_proc_reader_external, find_value_int8_in_stack) {
    test_template_external_find_value<std::int8_t>("stack");
}


TEST_F(ut_proc_reader_external, find_value_int8_in_heap) {
    test_template_external_find_value<std::int8_t>("heap");
}


TEST_F(ut_proc_reader_external, find_value_uint8_in_global) {
    test_template_external_find_value<std::uint8_t>("global");
}


TEST_F(ut_proc_reader_external, find_value_uint8_in_stack) {
    test_template_external_find_value<std::uint8_t>("stack");
}


TEST_F(ut_proc_reader_external, find_value_uint8_in_heap) {
    test_template_external_find_value<std::uint8_t>("heap");
}


TEST_F(ut_proc_reader_external, find_value_uint64_in_global) {
    test_template_external_find_value<std::uint64_t>("global");
}


TEST_F(ut_proc_reader_external, find_value_uint64_in_stack) {
    test_template_external_find_value<std::uint64_t>("stack");
}


TEST_F(ut_proc_reader_external, find_value_uint64_in_heap) {
    test_template_external_find_value<std::uint64_t>("heap");
}


TEST_F(ut_proc_reader_external, find_value_double_in_global) {
    test_template_external_find_value<double>("global");
}


TEST_F(ut_proc_reader_external, find_value_double_in_stack) {
    test_template_external_find_value<double>("stack");
}


TEST_F(ut_proc_reader_external, find_value_double_in_heap) {
    test_template_external_find_value<double>("heap");
}