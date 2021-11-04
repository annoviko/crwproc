/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once


#include <ostream>
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

class event_address { };

class event_done { };

class event_error { };

class event_exit { };

class event_save { };

class event_load { };

class event_revert { };

class event_dump { };


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
    event_address, 
    event_done, 
    event_error, 
    event_exit,
    event_save,
    event_load,
    event_revert,
    event_dump
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


std::ostream& operator<<(std::ostream& p_stream, const event& p_event);
std::ostream& operator<<(std::ostream& p_stream, const event_choose& p_event);
std::ostream& operator<<(std::ostream& p_stream, const event_filter& p_event);
std::ostream& operator<<(std::ostream& p_stream, const event_update& p_event);
std::ostream& operator<<(std::ostream& p_stream, const event_show& p_event);
std::ostream& operator<<(std::ostream& p_stream, const event_add& p_event);
std::ostream& operator<<(std::ostream& p_stream, const event_edit& p_event);
std::ostream& operator<<(std::ostream& p_stream, const event_set& p_event);
std::ostream& operator<<(std::ostream& p_stream, const event_refresh& p_event);
std::ostream& operator<<(std::ostream& p_stream, const event_remove& p_event);
std::ostream& operator<<(std::ostream& p_stream, const event_help& p_event);
std::ostream& operator<<(std::ostream& p_stream, const event_address& p_event);
std::ostream& operator<<(std::ostream& p_stream, const event_done& p_event);
std::ostream& operator<<(std::ostream& p_stream, const event_error& p_event);
std::ostream& operator<<(std::ostream& p_stream, const event_exit& p_event);
std::ostream& operator<<(std::ostream& p_stream, const event_save& p_event);
std::ostream& operator<<(std::ostream& p_stream, const event_load& p_event);
std::ostream& operator<<(std::ostream& p_stream, const event_revert& p_event);
std::ostream& operator<<(std::ostream& p_stream, const event_dump& p_event);
