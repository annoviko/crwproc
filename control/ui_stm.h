/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once

#include "context.h"
#include "state.h"


class ui_stm {
private:
    context m_context;
    state m_state = state_choose_proc{};
    event m_event;

public:
    void handle_state();
};

