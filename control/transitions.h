/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once

#include "event.h"
#include "state.h"


class transitions {
public:
    state operator()(state_choose_proc&, event_done&) const { return state_create_filter{}; }

    state operator()(state_create_filter&, event_done&) const { return state_search{}; }

    state operator()(state_update_filter&, event_done&) const { return state_search{}; }

    template <typename TypeState>
    state operator()(TypeState& state, event_refresh&) const { return state; }

    template <typename TypeState>
    state operator()(TypeState&, event_address&) const { return state_address{}; }

    template <typename TypeState>
    state operator()(TypeState&, event_filter&) const { return state_create_filter{}; }

    template <typename TypeState>
    state operator()(TypeState&, event_edit&) const { return state_edit{}; }

    template <typename TypeState>
    state operator()(TypeState&, event_show&) const { return state_show_search_result{}; }

    template <typename TypeState>
    state operator()(TypeState&, event_update&) const { return state_update_filter{}; }

    template <typename TypeState>
    state operator()(TypeState&, event_choose&) const { return state_choose_proc{}; }

    template <typename TypeState>
    state operator()(TypeState&, event_help&) const { return state_help{}; }

    template <typename TypeState>
    state operator()(TypeState&, event_exit&) const { return state_exit{}; }

    template <typename TypeState>
    state operator()(TypeState&, event_dump&) const { return state_dump{}; }

    template <typename TypeState, typename TypeEvent>
    state operator()(TypeState& state, TypeEvent&) const { return state; }
};