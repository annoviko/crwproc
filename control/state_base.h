/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once

#include "context.h"
#include "event.h"


class state_base {
protected:
    static event ask_next_action(const context& p_context);
};
