#pragma once

/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/

#pragma once


#include <string>
#include <optional>


namespace crwproc {

namespace string
{


template <typename TypeCastValue, typename TypeOriginalValue>
std::optional<TypeCastValue> __cast_numerical_value(const std::string& p_string, const std::size_t p_amount_symbols, const TypeOriginalValue p_original) {
    const TypeCastValue value_to_return = static_cast<TypeCastValue>(p_original);
    if (static_cast<TypeOriginalValue>(value_to_return) != p_original) {
        return { };
    }

    return (p_amount_symbols == p_string.size()) ? value_to_return : std::optional<TypeCastValue>{ };
}


template <typename TypeValue>
std::optional<TypeValue> stonum(const std::string& p_value, const int p_base = 10) try {
    std::size_t amount_symbols = 0;

    if constexpr (std::is_integral_v<TypeValue>) {
        if constexpr (std::is_signed_v<TypeValue>) {
            const auto value = std::stoll(p_value, &amount_symbols, p_base);
            return __cast_numerical_value<TypeValue>(p_value, amount_symbols, value);
        }
        else {
            const auto value = std::stoull(p_value, &amount_symbols, p_base);
            return __cast_numerical_value<TypeValue>(p_value, amount_symbols, value);
        }
    }
    else if (std::is_floating_point_v<TypeValue>) {
        if constexpr (std::is_same_v<TypeValue, float>) {
            const float value = std::stof(p_value, &amount_symbols);
            return (amount_symbols == p_value.size()) ? value : std::optional<TypeValue>{ };
        }
        else {
            const double value = std::stod(p_value, &amount_symbols);
            return (amount_symbols == p_value.size()) ? value : std::optional<TypeValue>{ };
        }
    }
}
catch (std::exception&) {
    return { };
}


}

}