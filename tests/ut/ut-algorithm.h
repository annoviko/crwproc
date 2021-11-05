/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once


#include <gtest/gtest.h>

#include <vector>

#include "core/algorithm.h"


template <typename TypePattern>
void test_template_kmp_pattern_map(const TypePattern& p_pattern, const std::size_t p_length, const std::vector<std::size_t>& p_expected_map) {
    std::vector<std::size_t> actual_map(p_length, 0);
    crwproc::algorithm::kmp_pattern_map(p_pattern, p_length, actual_map);
    EXPECT_EQ(actual_map, p_expected_map);
}


template <typename TypeInput, typename TypePattern>
void test_template_kmp(const TypeInput& p_input, const std::size_t p_input_length, const TypePattern& p_pattern, const std::size_t p_pattern_length, const std::size_t p_position) {
    const std::size_t actual_position = crwproc::algorithm::kmp(p_input, p_input_length, p_pattern, p_pattern_length);
    EXPECT_EQ(p_position, actual_position);
}
