/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "ut-type_desc.h"


TEST(ut_type_desc, default_construction) {
    type_desc type;

    ASSERT_EQ(value_type::invalid, type.get_type());
    ASSERT_EQ(0, type.get_size());
    ASSERT_FALSE(type.is_valid());
}


TEST(ut_type_desc, create_integer_signed) {
    test_template_create_type<std::int8_t>();
    test_template_create_type<std::int16_t>();
    test_template_create_type<std::int32_t>();
    test_template_create_type<std::int64_t>();
}


TEST(ut_type_desc, create_integer_unsigned) {
    test_template_create_type<std::uint8_t>();
    test_template_create_type<std::uint16_t>();
    test_template_create_type<std::uint32_t>();
    test_template_create_type<std::uint64_t>();
}


TEST(ut_type_desc, create_floating) {
    test_template_create_type<float>();
    test_template_create_type<double>();
}


void test_template_construct(const value_type& p_type, const std::size_t p_size, const bool p_signed) {
    type_desc type(p_type, p_size, p_signed);

    ASSERT_EQ(p_type, type.get_type());
    ASSERT_EQ(p_size, type.get_size());
    ASSERT_EQ(p_signed, type.is_signed());

    ASSERT_EQ(p_type != value_type::invalid, type.is_valid());
}


TEST(ut_type_desc, construct_invalid) {
    test_template_construct(value_type::invalid, 1, false);
}


TEST(ut_type_desc, construct_valid) {
    test_template_construct(value_type::integral, 1, false);
    test_template_construct(value_type::integral, 1, true);
    test_template_construct(value_type::floating, sizeof(float), true);
    test_template_construct(value_type::doubling, sizeof(double), true);
}


TEST(ut_type_desc, construct_valid_integer_wierd) {
    test_template_construct(value_type::integral, 100, false);
}


TEST(ut_type_desc, construct_float_autocorrection) {
    type_desc type(value_type::floating, 100, false);

    ASSERT_EQ(sizeof(float), type.get_size());
    ASSERT_EQ(true, type.is_signed());
}


TEST(ut_type_desc, construct_double_autocorrection) {
    type_desc type(value_type::doubling, 56, false);

    ASSERT_EQ(sizeof(double), type.get_size());
    ASSERT_EQ(true, type.is_signed());
}
