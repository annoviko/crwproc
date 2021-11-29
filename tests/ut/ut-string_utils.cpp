/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include <gtest/gtest.h>

#include "core/string_utils.h"

#include <string>


void test_template_trim_left(const std::string& p_input, const std::string& p_expected) {
    std::string actual = p_input;
    crwproc::string::utils::trim_left(actual);
    EXPECT_EQ(p_expected, actual);
}


TEST(ut_string_utils, trim_left_single_word) {
    test_template_trim_left("   left", "left");
    test_template_trim_left("   left  ", "left  ");
    test_template_trim_left("   left   ", "left   ");
    test_template_trim_left("left", "left");
    test_template_trim_left("left  ", "left  ");
}


TEST(ut_string_utils, trim_left_several_words) {
    test_template_trim_left("   left and right", "left and right");
    test_template_trim_left("   left and right  ", "left and right  ");
    test_template_trim_left("   left  and   right   ", "left  and   right   ");
    test_template_trim_left("left and right", "left and right");
    test_template_trim_left("left and right  ", "left and right  ");
}


void test_template_trim_right(const std::string& p_input, const std::string& p_expected) {
    std::string actual = p_input;
    crwproc::string::utils::trim_right(actual);
    EXPECT_EQ(p_expected, actual);
}


TEST(ut_string_utils, trim_right_single_word) {
    test_template_trim_right("   right", "   right");
    test_template_trim_right("   right  ", "   right");
    test_template_trim_right("   right   ", "   right");
    test_template_trim_right("right", "right");
    test_template_trim_right("right  ", "right");
    test_template_trim_right(" right   ", " right");
    test_template_trim_right("  right    ", "  right");
}


TEST(ut_string_utils, trim_right_several_words) {
    test_template_trim_right("   right and left", "   right and left");
    test_template_trim_right("   right and left  ", "   right and left");
    test_template_trim_right("   right and left   ", "   right and left");
    test_template_trim_right("right and left", "right and left");
    test_template_trim_right("right and left  ", "right and left");
    test_template_trim_right(" right and left   ", " right and left");
    test_template_trim_right("  right and left    ", "  right and left");
}


void test_template_trim(const std::string& p_input, const std::string& p_expected) {
    std::string actual = p_input;
    crwproc::string::utils::trim(actual);
    EXPECT_EQ(p_expected, actual);
}


TEST(ut_string_utils, trim_single_word) {
    test_template_trim("   right", "right");
    test_template_trim("   right  ", "right");
    test_template_trim("   right   ", "right");
    test_template_trim("right", "right");
    test_template_trim("right  ", "right");
    test_template_trim(" right   ", "right");
    test_template_trim("  right    ", "right");
}


TEST(ut_string_utils, trim_several_words) {
    test_template_trim("   right and left", "right and left");
    test_template_trim("   right and left  ", "right and left");
    test_template_trim("   right and left   ", "right and left");
    test_template_trim("right and left", "right and left");
    test_template_trim("right and left  ", "right and left");
    test_template_trim(" right and left   ", "right and left");
    test_template_trim("  right and left    ", "right and left");
}
