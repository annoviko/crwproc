/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once


#include <type_traits>
#include <variant>

#include "core/traits.h"


class event_choose { };

class event_filter { };

class event_update { };

class event_show { };

class event_add { };

class event_edit { };

class event_set { };

class event_refresh { };

class event_remove { };

class event_help { };

class event_done { };

class event_error { };

class event_exit { };


using event = std::variant<
    event_choose, 
    event_filter, 
    event_update,
    event_show, 
    event_add, 
    event_edit, 
    event_set, 
    event_refresh, 
    event_remove, 
    event_help, 
    event_done, 
    event_error, 
    event_exit
>;


template <typename ... ExpectedEvents>
bool is_event_type(const event& p_event) {
    return std::visit([](auto&& instance) {
        using actual_type = std::decay_t<decltype(instance)>;
        return crwproc::traits::is_any<actual_type, ExpectedEvents...>::value;
    }, p_event);
}


template <typename ... ExpectedEvent, typename FunctionType>
event continue_until_event_is_not(const FunctionType& p_function) {
    event current_event = p_function();
    while (is_event_type<ExpectedEvent...>(current_event)) {
        current_event = p_function();
    }

    return current_event;
}

