/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once

#include <exception>
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

    static const std::unordered_map<filter_type, std::string> FILTER_TYPE_SHORT_DICT;

public:
    event operator()(context& p_context) const;

private:
    static void create_filter(context& p_context);

    static filter_type ask_filter_type();

public:
    friend std::ostream& operator<<(std::ostream& p_stream, const state_create_filter& p_state);

    friend std::ostream& operator<<(std::ostream& p_stream, const filter_type& p_state);

private:
    template <typename TypeFilter>
    static void ask_filter(context& p_context) {
        type_desc type = asker::ask_type_desc();

        if constexpr (std::is_same<TypeFilter, filter_equal>::value) {
            p_context.get_filter() = TypeFilter(type);

            while (!filter_reader_value::read(p_context.get_filter())) { }
        }
        else if constexpr (std::is_same<TypeFilter, filter_range>::value) {
            p_context.get_filter() = filter_range(type);

            while (!filter_reader_value::read(p_context.get_filter())) { }
        }
        else if constexpr (crwproc::traits::is_any<TypeFilter, filter_more, filter_less>::value) {
            p_context.get_filter() = TypeFilter(type);
        }
    }
};