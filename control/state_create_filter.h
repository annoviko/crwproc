/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once

#include <optional>
#include <unordered_map>

#include "core/filter.h"

#include "context.h"
#include "event.h"
#include "filter_reader_value.h"


class state_create_filter {
private:
    enum class filter_type {
        equal = 0,
        range,
        last_item
    };

private:
    static const std::size_t INVALID_SIZE;
    static const std::size_t INVALID_SIGN;

    static const std::unordered_map<filter_type, std::string> FILTER_TYPE_DICT;

public:
    event operator()(context& p_context);

private:
    void create_filter(context& p_context) const;

    filter_type ask_filter_type() const;

    value::type ask_value_type() const;

    std::size_t ask_value_size() const;

    std::optional<bool> ask_value_sign() const;

private:
    template <typename TypeFilter>
    void ask_filter(context& p_context) const {
        value::type type = ask_value_type();
        while (type == value::type::invalid) {
            type = ask_value_type();
        }

        std::size_t size = 0;
        bool sign = true;
        if (type == value::type::integral) {
            size = ask_value_size();
            while (size == INVALID_SIZE) {
                size = ask_value_size();
            }

            std::optional<bool> is_signed = ask_value_sign();
            while (!is_signed.has_value()) {
                is_signed = ask_value_sign();
            }

            sign = is_signed.value();
        }

        value variable(type, size, sign, std::string("0"));

        if constexpr (std::is_same<TypeFilter, filter_equal>::value) {
            p_context.get_filter() = filter_equal(variable);
        }
        else if constexpr (std::is_same<TypeFilter, filter_range>::value) {
            p_context.get_filter() = filter_range(variable, variable);
        }
        else {
            static_assert(false);
        }

        while (!filter_reader_value::read(p_context.get_filter())) {}
    }
};