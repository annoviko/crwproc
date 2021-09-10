/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include <gtest/gtest.h>

#include "core/traits.h"


TEST(traits, is_any_has_type) {
    if constexpr (!crwproc::traits::is_any<std::int8_t, std::int64_t, std::int32_t, std::int16_t, std::int8_t>::value) {
        FAIL() << "int8_t is not found among other types where it exists";
    }
}


TEST(traits, is_any_has_not_type) {
    if constexpr (crwproc::traits::is_any<std::int8_t, std::int64_t, std::int32_t, std::int16_t>::value) {
        FAIL() << "int8_t is found among other types where it does not exist";
    }
}