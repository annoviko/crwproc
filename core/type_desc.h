/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once


#include <cstddef>
#include <string>
#include <type_traits>

#include "value_type.h"


class type_desc {
private:
    value_type      m_type = value_type::invalid;
    std::size_t     m_size = 0;
    bool            m_signed = false;

public:
    type_desc() = default;

    type_desc(const value_type p_type, const std::size_t p_size, const bool p_signed);

public:
    bool is_valid() const;

    value_type get_type() const;

    std::size_t get_size() const;

    bool is_signed() const;

    std::string to_string() const;

private:
    template <typename Type>
    static value_type get_type() {
        if constexpr (std::is_integral<std::remove_reference<Type>::type>::value) {
            return value_type::integral;
        }
        else if constexpr (std::is_same<double, std::remove_reference<Type>::type>::value) {
            return value_type::doubling;
        }
        else if constexpr (std::is_same<float, std::remove_reference<Type>::type>::value) {
            return value_type::floating;
        }
        else {
            static_assert(false);
        }
    }

public:
    template <typename Type>
    static type_desc create() {
        const std::size_t size = sizeof(Type);
        const bool is_signed_value = std::is_signed<Type>::value;
        const value_type type = get_type<Type>();

        return type_desc(type, size, is_signed_value);
    }

public:
    bool operator==(const type_desc& p_type) const;

    friend std::ostream& operator<<(std::ostream& p_stream, const type_desc& p_type);
};