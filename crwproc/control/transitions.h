#pragma once

#include "event.h"
#include "state.h"


class transitions {
public:
    state operator()(state_choose_proc& state, event_done&) { return state_create_filter{}; }

    state operator()(state_create_filter& state, event_done&) { return state_search{}; }

    template <typename TypeState>
    state operator()(TypeState& state, event_choose&) { return state_choose_proc{}; }

    template <typename TypeState>
    state operator()(TypeState& state, event_exit&) { return state_exit{}; }

    template <typename TypeState, typename TypeEvent>
    state operator()(TypeState& state, TypeEvent&) { return state; }
};