#pragma once

#include "context.h"
#include "event.h"


class state_base {
protected:
    event ask_next_action(context& p_context);
};
