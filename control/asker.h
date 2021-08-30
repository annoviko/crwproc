/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once

#include <functional>
#include <limits>

class asker {
public:
    using action_index = std::function<void(std::size_t)>;

public:
    static constexpr std::size_t INVALID_INDEX = std::numeric_limits<std::size_t>::max();

public:
    static std::size_t ask_index(const std::size_t p_limit, const action_index& p_action = nullptr);
};