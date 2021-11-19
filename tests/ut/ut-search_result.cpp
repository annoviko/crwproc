/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include <gtest/gtest.h>

#include "core/search_result.h"


void test_template_search_result(const std::vector<std::size_t>& p_block_sizes, const type_desc p_type) {
    std::uint64_t current_address = 1;

    std::size_t expected_amount_elements = 0;

    search_result result(p_type);
    for (const std::size_t block_size : p_block_sizes) {
        for (std::size_t i = 0; i < block_size; i++) {
            result.get_memory_blocks().back().get_values().push_back(proc_pointer{ current_address++ });
            expected_amount_elements++;
        }
    }

    const std::size_t expected_size_in_bytes = expected_amount_elements * p_type.get_size();

    EXPECT_EQ(expected_amount_elements, result.get_amount_values());
    EXPECT_EQ(p_block_sizes.size(), result.get_amount_memory_blocks());
    EXPECT_EQ(expected_size_in_bytes, result.get_size());

    /* test iterators */
    std::uint64_t expected_address = 1;
    std::size_t total_amount_elements = 0;

    for (auto iter = result.begin(); iter != result.end(); ++iter) {
        EXPECT_EQ(expected_address++, iter->get_address());
        total_amount_elements++;
    }

    EXPECT_EQ(expected_amount_elements, total_amount_elements);

    /* test for-each */
    expected_address = 1;
    total_amount_elements = 0;

    for (const proc_pointer & pointer : result) {
        EXPECT_EQ(expected_address++, pointer.get_address());
        total_amount_elements++;
    }

    EXPECT_EQ(expected_amount_elements, total_amount_elements);
}


TEST(ut_search_result, empty) {
    test_template_search_result({ }, type_desc::create<std::uint8_t>());
}

TEST(ut_search_result, blocks_1_values_1_byte_1) {
    test_template_search_result({ 1 }, type_desc::create<std::uint8_t>());
}


TEST(ut_search_result, blocks_1_values_1_byte_8) {
    test_template_search_result({ 1 }, type_desc::create<std::uint64_t>());
}


TEST(ut_search_result, blocks_1_values_5_byte_1) {
    test_template_search_result({ 5 }, type_desc::create<std::uint8_t>());
}


TEST(ut_search_result, blocks_1_values_5_byte_8) {
    test_template_search_result({ 5 }, type_desc::create<std::uint64_t>());
}


TEST(ut_search_result, blocks_1_values_n_byte_1) {
    test_template_search_result({ 16 }, type_desc::create<std::uint8_t>());
    test_template_search_result({ 32 }, type_desc::create<std::uint8_t>());
    test_template_search_result({ 64 }, type_desc::create<std::uint8_t>());
}


TEST(ut_search_result, blocks_1_values_n_byte_x) {
    test_template_search_result({ 8 }, type_desc::create<std::uint8_t>());
    test_template_search_result({ 16 }, type_desc::create<std::uint16_t>());
    test_template_search_result({ 32 }, type_desc::create<std::uint32_t>());
    test_template_search_result({ 64 }, type_desc::create<std::uint64_t>());
}


TEST(ut_search_result, blocks_2_values_n_byte_8) {
    test_template_search_result({ 1, 1 }, type_desc::create<std::uint8_t>());
    test_template_search_result({ 1, 8 }, type_desc::create<std::uint8_t>());
    test_template_search_result({ 2, 4 }, type_desc::create<std::uint8_t>());
}


TEST(ut_search_result, blocks_3_values_n_byte_8) {
    test_template_search_result({ 1, 1, 1 }, type_desc::create<std::uint8_t>());
    test_template_search_result({ 1, 8, 4 }, type_desc::create<std::uint8_t>());
    test_template_search_result({ 2, 4, 16 }, type_desc::create<std::uint8_t>());
}


TEST(ut_search_result, blocks_n_values_x_byte_y) {
    test_template_search_result({ 20 }, type_desc::create<double>());
    test_template_search_result({ 1, 1, 1 }, type_desc::create<std::uint16_t>());
    test_template_search_result({ 1, 8, 4, 7, 10 }, type_desc::create<std::uint32_t>());
    test_template_search_result({ 2, 4, 16, 1, 2, 10, 1 }, type_desc::create<std::uint64_t>());
}
