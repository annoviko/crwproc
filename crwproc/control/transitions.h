#pragma once

#include "event.h"
#include "state.h"


class transitions {
public:
    state operator()(state_choose_proc&, event_done&) { return state_create_filter{}; }

    state operator()(state_create_filter&, event_done&) { return state_search{}; }

    state operator()(state_search&, event_filter&) { return state_create_filter{}; }

    state operator()(state_show_search_result&, event_filter&) { return state_create_filter{}; }

    state operator()(state_update_filter_value&, event_done&) { return state_search{}; }

    template <typename TypeState>
    state operator()(TypeState& state, event_refresh&) { return state; }

    template <typename TypeState>
    state operator()(TypeState& state, event_edit&) { return state_edit{}; }

    template <typename TypeState>
    state operator()(TypeState& state, event_show&) { return state_show_search_result{}; }

    template <typename TypeState>
    state operator()(TypeState& state, event_update&) { return state_update_filter_value{}; }

    template <typename TypeState>
    state operator()(TypeState& state, event_choose&) { return state_choose_proc{}; }

    template <typename TypeState>
    state operator()(TypeState& state, event_help&) { return state_help{}; }

    template <typename TypeState>
    state operator()(TypeState& state, event_exit&) { return state_exit{}; }

    template <typename TypeState, typename TypeEvent>
    state operator()(TypeState& state, TypeEvent&) { return state; }
};