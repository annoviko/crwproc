/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once

#include <cstdint>


class storage {
public:
    std::int8_t  i8 = 0;
    std::int16_t i16 = 0;
    std::int32_t i32 = 0;
    std::int64_t i64 = 0;

    std::uint8_t  u8 = 0;
    std::uint16_t u16 = 0;
    std::uint32_t u32 = 0;
    std::uint64_t u64 = 0;

    float flt = static_cast<float>(0.0);
    double dbl = 0.0;

public:
    storage();
};
