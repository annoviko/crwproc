#pragma once

#include "event.h"
#include "state.h"


class transitions {
public:
    template <typename TypeState>
    state operator()(TypeState& state, event_exit&) { return state_exit{}; }

    template <typename TypeState, typename TypeEvent>
    state operator()(TypeState& state, TypeEvent&) { return state; }
};