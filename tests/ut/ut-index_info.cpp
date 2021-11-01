/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include <gtest/gtest.h>

#include <string>

#include "control/index_info.h"


void template_test_index_info(const std::string& p_instruction, const std::size_t p_limit, const bool p_index_only, const bool expected_valid, const std::size_t expected_begin = 0, const std::size_t expected_end = 0) {
    const index_info status(p_instruction, p_limit, p_index_only);

    EXPECT_EQ(expected_valid, status.is_valid());

    if (expected_valid) {
        EXPECT_EQ(expected_begin, status.get_begin());
        EXPECT_EQ(expected_end, status.get_end());
        EXPECT_TRUE(status.reason().empty());
    }
    else {
        EXPECT_FALSE(status.reason().empty());
    }
}


TEST(ut_index_info, all_invalid_empty) {
    template_test_index_info("all", 0, false, false);
}

TEST(ut_index_info, all_01) {
    template_test_index_info("all", 1, false, true, 0, 1);
}

TEST(ut_index_info, all_03) {
    template_test_index_info("all", 3, false, true, 0, 3);
}

TEST(ut_index_info, all_10) {
    template_test_index_info("all", 10, false, true, 0, 10);
}

TEST(ut_index_info, 1half_01) {
    template_test_index_info("1half", 1, false, true, 0, 1);
}

TEST(ut_index_info, 2half_01) {
    template_test_index_info("2half", 1, false, false);
}

TEST(ut_index_info, 1half_02) {
    template_test_index_info("1half", 2, false, true, 0, 1);
}

TEST(ut_index_info, 2half_02) {
    template_test_index_info("2half", 2, false, true, 1, 2);
}

TEST(ut_index_info, 1half_03) {
    template_test_index_info("1half", 3, false, true, 0, 2);
}

TEST(ut_index_info, 2half_03) {
    template_test_index_info("2half", 3, false, true, 2, 3);
}

TEST(ut_index_info, 1half_10) {
    template_test_index_info("1half", 10, false, true, 0, 5);
}

TEST(ut_index_info, 2half_10) {
    template_test_index_info("2half", 10, false, true, 5, 10);
}

TEST(ut_index_info, range_0_1) {
    template_test_index_info("0-1", 1, false, true, 0, 1);
}

TEST(ut_index_info, range_0_10) {
    template_test_index_info("0-10", 10, false, true, 0, 10);
}

TEST(ut_index_info, range_5_10) {
    template_test_index_info("5-10", 10, false, true, 5, 10);
}

TEST(ut_index_info, range_3_6) {
    template_test_index_info("3-6", 10, false, true, 3, 6);
}

TEST(ut_index_info, range_end_out_of_range) {
    template_test_index_info("6-11", 10, false, false);
}

TEST(ut_index_info, range_both_out_of_range) {
    template_test_index_info("10-15", 10, false, false);
}

TEST(ut_index_info, value_00) {
    template_test_index_info("0", 10, false, true, 0, 1);
}

TEST(ut_index_info, value_01) {
    template_test_index_info("1", 10, false, true, 1, 2);
}

TEST(ut_index_info, value_05) {
    template_test_index_info("5", 10, false, true, 5, 6);
}

TEST(ut_index_info, value_out_of_range) {
    template_test_index_info("15", 10, false, false);
}
