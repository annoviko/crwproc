#pragma once

#include "context.h"
#include "event.h"


class state_exit {
public:
    event operator()(context& p_context);
};