/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include <gtest/gtest.h>

#include "core/string_split.h"


void test_template_split_string(const std::string& p_message, const std::vector<std::string>& p_expected) {
    std::vector<std::string> actual = { };

    crwproc::string::split(p_message, std::back_inserter(actual));
    ASSERT_EQ(p_expected, actual);

}


TEST(ut_string, string_split_one_space) {
    test_template_split_string("one two three four", { "one", "two", "three", "four" });
    test_template_split_string("one two three four five", { "one", "two", "three", "four", "five"});
}


TEST(ut_string, string_split_two_spaces) {
    test_template_split_string("one  two  three  four", { "one", "two", "three", "four" });
    test_template_split_string("one  two  three  four  five", { "one", "two", "three", "four", "five"});
}


TEST(ut_string, string_split_several_spaces) {
    test_template_split_string("one    two three  four", { "one", "two", "three", "four" });
    test_template_split_string("one    two three  four   five", { "one", "two", "three", "four", "five" });
}


TEST(ut_string, string_split_one_word) {
    test_template_split_string("one", { "one" });
}


TEST(ut_string, string_split_one_word_spaces_before) {
    test_template_split_string(" one", { "one" });
    test_template_split_string("    one", { "one" });
    test_template_split_string("      one", { "one" });
}


TEST(ut_string, string_split_one_word_spaces_after) {
    test_template_split_string("one ", { "one" });
    test_template_split_string("one    ", { "one" });
    test_template_split_string("one  ", { "one" });
}


TEST(ut_string, string_split_one_word_spaces_before_after) {
    test_template_split_string(" one ", { "one" });
    test_template_split_string("   one    ", { "one" });
    test_template_split_string(" one  ", { "one" });
}


TEST(ut_string, string_split_multiline) {
    test_template_split_string("one two three four\nfive six\nseven", { "one", "two", "three", "four", "five", "six", "seven" });
    test_template_split_string("one two  \n three four \nfive six\n seven", { "one", "two", "three", "four", "five", "six", "seven" });
}


TEST(ut_string, string_split_empty_lines) {
    test_template_split_string("one two\n \n\n three four\n\n\nfive six\nseven\n\n  \n", { "one", "two", "three", "four", "five", "six", "seven" });
    test_template_split_string("\none\n\ntwo  \n\n three\nfour \nfive  \n\n six\n seven\n\n \n  \n", { "one", "two", "three", "four", "five", "six", "seven" });
}


TEST(ut_string, string_split_empty_input) {
    test_template_split_string("", { });
    test_template_split_string("    ", { });
    test_template_split_string("  \n\n  \n", { });
}
