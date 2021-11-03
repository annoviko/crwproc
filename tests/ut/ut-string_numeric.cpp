/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include <gtest/gtest.h>

#include "core/string_numeric.h"


TEST(ut_string_numeric, correct_numbers) {
    EXPECT_EQ(static_cast<std::int8_t>(-16), crwproc::string::stonum<std::int8_t>("-16").value());
    EXPECT_EQ(static_cast<std::int16_t>(-1024), crwproc::string::stonum<std::int16_t>("-1024").value());
    EXPECT_EQ(static_cast<std::int32_t>(-1048576), crwproc::string::stonum<std::int32_t>("-1048576").value());
    EXPECT_EQ(static_cast<std::int64_t>(-8589934592), crwproc::string::stonum<std::int64_t>("-8589934592").value());

    EXPECT_EQ(static_cast<std::uint8_t>(16), crwproc::string::stonum<std::uint8_t>("16").value());
    EXPECT_EQ(static_cast<std::uint16_t>(1024), crwproc::string::stonum<std::uint16_t>("1024").value());
    EXPECT_EQ(static_cast<std::uint32_t>(1048576), crwproc::string::stonum<std::uint32_t>("1048576").value());
    EXPECT_EQ(static_cast<std::uint64_t>(8589934592), crwproc::string::stonum<std::uint64_t>("8589934592").value());

    EXPECT_EQ(static_cast<float>(3.14), crwproc::string::stonum<float>("3.14").value());
    EXPECT_EQ(static_cast<double>(-3.14), crwproc::string::stonum<double>("-3.14").value());
}


TEST(ut_string_numeric, out_of_range) {
    EXPECT_FALSE(crwproc::string::stonum<std::int8_t>("128").has_value());
    EXPECT_FALSE(crwproc::string::stonum<std::int16_t>("32769").has_value());
    EXPECT_FALSE(crwproc::string::stonum<std::int32_t>("2147483649").has_value());

    EXPECT_FALSE(crwproc::string::stonum<std::uint8_t>("257").has_value());
    EXPECT_FALSE(crwproc::string::stonum<std::uint16_t>("65537").has_value());
    EXPECT_FALSE(crwproc::string::stonum<std::uint32_t>("4294967297").has_value());
}


TEST(ut_string_numeric, wrong_value) {
    EXPECT_FALSE(crwproc::string::stonum<std::int8_t>("12asd").has_value());
    EXPECT_FALSE(crwproc::string::stonum<std::int16_t>("327ewr").has_value());
    EXPECT_FALSE(crwproc::string::stonum<std::int32_t>("214748qwer").has_value());
    EXPECT_FALSE(crwproc::string::stonum<std::int64_t>("2147dsqwer").has_value());

    EXPECT_FALSE(crwproc::string::stonum<std::uint8_t>("25qwe").has_value());
    EXPECT_FALSE(crwproc::string::stonum<std::uint16_t>("655asd").has_value());
    EXPECT_FALSE(crwproc::string::stonum<std::uint32_t>("4294dfsdf").has_value());
    EXPECT_FALSE(crwproc::string::stonum<std::uint64_t>("42w4dfsdf").has_value());

    EXPECT_FALSE(crwproc::string::stonum<float>("6.55asd").has_value());
    EXPECT_FALSE(crwproc::string::stonum<double>("4.294dfsdf").has_value());
}
