/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include <gtest/gtest.h>

#include <filesystem>

#include "control/edit_table_reader.h"
#include "control/edit_table_writer.h"


TEST(ut_edit_table_reader, one_element) {
    edit_table table;
    const bool result = edit_table_reader("resources/edit_table_one_element.json").read(table);

    EXPECT_TRUE(result);
    EXPECT_EQ(std::size_t(1), table.size());
    EXPECT_EQ(std::uint64_t(741459622180), table[0].get_pointer().get_address());
    EXPECT_EQ("cash", table[0].get_name());
    EXPECT_EQ(type_desc(value_type::integral, 4, true), table[0].get_type());
}


TEST(ut_edit_table_reader, two_elements) {
    edit_table table;
    const bool result = edit_table_reader("resources/edit_table_two_elements.json").read(table);

    EXPECT_TRUE(result);
    EXPECT_EQ(std::size_t(2), table.size());
    EXPECT_EQ(std::uint64_t(741459622180), table[0].get_pointer().get_address());
    EXPECT_EQ("cash", table[0].get_name());
    EXPECT_EQ(type_desc(value_type::doubling, sizeof(double), true), table[0].get_type());

    EXPECT_EQ(std::uint64_t(741459622184), table[1].get_pointer().get_address());
    EXPECT_EQ("wood", table[1].get_name());
    EXPECT_EQ(type_desc(value_type::integral, 2, false), table[1].get_type());
}


TEST(ut_edit_table_reader, invalid_json) {
    edit_table table;
    const bool result = edit_table_reader("resources/edit_table_invalid_json.json").read(table);

    EXPECT_FALSE(result);
    EXPECT_EQ(std::size_t(0), table.size());
}


TEST(ut_edit_table_reader, empty) {
    edit_table table;
    const bool result = edit_table_reader("resources/edit_table_empty.json").read(table);

    EXPECT_TRUE(result);
    EXPECT_EQ(std::size_t(0), table.size());
}



class ut_edit_table_writer : public ::testing::Test {
private:
    constexpr static char filename[] = "ut_edit_table.json";

protected:
    void SetUp() override { }


    void TearDown() override {
        std::filesystem::remove(filename);
    }


    void TestTemplate(const edit_table& p_expected_table) const {
        bool result = edit_table_writer(filename).write(p_expected_table);

        EXPECT_TRUE(result);

        edit_table actual_table;
        result = edit_table_reader(filename).read(actual_table);

        EXPECT_TRUE(result);
        EXPECT_EQ(p_expected_table, actual_table);
    }
};



TEST_F(ut_edit_table_writer, empty) {
    edit_table table = { };
    TestTemplate(table);
}


TEST_F(ut_edit_table_writer, one_element) {
    edit_table table = { edit_table_entry{ "oil", proc_pointer(741459622180), { value_type::integral, 2, false } } };
    TestTemplate(table);
}


TEST_F(ut_edit_table_writer, two_elements) {
    edit_table table = {
        edit_table_entry{ "oil", proc_pointer(741459622180), { value_type::integral, 2, false } },
        edit_table_entry{ "wood", proc_pointer(741459622184), { value_type::integral, 4, false } }
    };
    TestTemplate(table);
}


TEST_F(ut_edit_table_writer, three_elements) {
    edit_table table = {
        edit_table_entry{ "oil", proc_pointer(741459622180), { value_type::integral, 2, false } },
        edit_table_entry{ "wood", proc_pointer(741459622184), { value_type::integral, 4, false } },
        edit_table_entry{ "cash", proc_pointer(741459622188), { value_type::doubling, 4, true } }
    };
    TestTemplate(table);
}

