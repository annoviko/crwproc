/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once

#include <unordered_map>

#include "core/filter.h"

#include "asker.h"
#include "context.h"
#include "event.h"
#include "filter_reader_value.h"


class state_create_filter {
private:
    enum class filter_type {
        equal = 0,
        range,
        more,
        less,
        last_item
    };

private:
    static const std::unordered_map<filter_type, std::string> FILTER_TYPE_DICT;

public:
    event operator()(context& p_context);

private:
    void create_filter(context& p_context) const;

    filter_type ask_filter_type() const;

private:
    template <typename TypeFilter>
    void ask_filter(context& p_context) const {
        value variable = asker::ask_blank_value();

        if constexpr (std::is_same<TypeFilter, filter_equal>::value) {
            p_context.get_filter() = TypeFilter(variable);

            while (!filter_reader_value::read(p_context.get_filter())) { }
        }
        else if constexpr (std::is_same<TypeFilter, filter_range>::value) {
            p_context.get_filter() = filter_range(variable, variable);

            while (!filter_reader_value::read(p_context.get_filter())) { }
        }
        else if constexpr (crwproc::traits::is_any<TypeFilter, filter_more, filter_less>::value) {
            p_context.get_filter() = TypeFilter(variable);
        }
        else {
            static_assert(false);
        }
    }
};