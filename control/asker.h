/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once


#include <functional>
#include <limits>
#include <optional>

#include "core/value.h"
#include "core/type_desc.h"


#undef max


class asker {
public:
    using action_index = std::function<void(std::size_t)>;

public:
    static constexpr std::size_t INVALID_INDEX = std::numeric_limits<std::size_t>::max();
    static constexpr std::size_t INVALID_SIZE = std::numeric_limits<std::size_t>::max();

public:
    static std::size_t ask_index(const std::size_t p_limit, const action_index& p_action, const bool p_interruptible = true);

    static std::size_t ask_index(const std::size_t p_limit, const bool p_interruptible = true);

    static value_type ask_value_type();

    static std::size_t ask_value_size();

    static std::optional<bool> ask_value_sign();

    static type_desc ask_type_desc();

    static std::optional<uint64_t> ask_address();
};