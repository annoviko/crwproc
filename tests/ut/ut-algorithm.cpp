/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "ut-algorithm.h"

#include <string>



TEST(ut_algorithm, kmp_pattern_map_with_string) {
    test_template_kmp_pattern_map(std::string("abcdabca"), 8, { 0, 0, 0, 0, 1, 2, 3, 1 });
    test_template_kmp_pattern_map(std::string("aabaabaaa"), 9, { 0, 1, 0, 1, 2, 3, 4, 5, 2 });
}


TEST(ut_algorithm, kmp_pattern_map_with_vector) {
    test_template_kmp_pattern_map(std::vector<std::size_t>{ 0, 1, 2, 3, 0, 1, 2, 0 }, 8, { 0, 0, 0, 0, 1, 2, 3, 1 });
    test_template_kmp_pattern_map(std::vector<std::size_t>{ 0, 0, 1, 0, 0, 1, 0, 0, 0 }, 9, { 0, 1, 0, 1, 2, 3, 4, 5, 2 });
}


TEST(ut_algorithm, kmp_pattern_map_with_array) {
    const std::uint8_t input[] = { 0, 1, 2, 3, 0, 1, 2, 0 };
    const std::vector<std::size_t> expected_map = { 0, 0, 0, 0, 1, 2, 3, 1 };
    test_template_kmp_pattern_map(input, std::size(input), expected_map);
}


TEST(ut_algorithm, kmp_with_string) {
    test_template_kmp(std::string("abcdabca"), 8, std::string("abcd"), 4, 0);
    test_template_kmp(std::string("abcdabca"), 8, std::string("bcd"), 3, 1);
    test_template_kmp(std::string("abcdabca"), 8, std::string("cdab"), 4, 2);
    test_template_kmp(std::string("abcdabca"), 8, std::string("dabca"), 5, 3);
    test_template_kmp(std::string("abcdabca"), 8, std::string("abca"), 4, 4);
    test_template_kmp(std::string("abcdabca"), 8, std::string("cdaf"), 4, crwproc::algorithm::npos);
}


TEST(ut_algorithm, kmp_with_byte_vector) {
    test_template_kmp(std::vector<std::uint8_t>{ 0, 1, 2, 3, 0, 1, 2, 0 }, 8, std::vector<std::uint8_t>{ 0, 1, 2 }, 3, 0);
    test_template_kmp(std::vector<std::uint8_t>{ 0, 1, 2, 3, 0, 1, 2, 0 }, 8, std::vector<std::uint8_t>{ 1, 2, 3 }, 3, 1);
    test_template_kmp(std::vector<std::uint8_t>{ 0, 1, 2, 3, 0, 1, 2, 0 }, 8, std::vector<std::uint8_t>{ 2, 3, 0, 1 }, 4, 2);
    test_template_kmp(std::vector<std::uint8_t>{ 0, 1, 2, 3, 0, 1, 2, 0 }, 8, std::vector<std::uint8_t>{ 3, 0, 1, 2, 0 }, 5, 3);
    test_template_kmp(std::vector<std::uint8_t>{ 0, 1, 2, 3, 0, 1, 2, 0 }, 8, std::vector<std::uint8_t>{ 0, 1, 2, 0 }, 4, 4);
    test_template_kmp(std::vector<std::uint8_t>{ 0, 1, 2, 3, 0, 1, 2, 0 }, 8, std::vector<std::uint8_t>{ 2, 3, 0, 8 }, 4, crwproc::algorithm::npos);
}


TEST(ut_algorithm, kmp_with_array) {
    const std::uint8_t input[] = { 0, 1, 2, 3, 0, 1, 2, 0 };
    const std::uint8_t pattern[] = { 2, 3, 0, 1 };
    test_template_kmp(input, std::size(input), pattern, std::size(pattern), 2);
}
