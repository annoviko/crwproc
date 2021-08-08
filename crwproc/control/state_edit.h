#pragma once

#include "context.h"
#include "event.h"


class state_edit {
public:
    event operator()(context& p_context);

private:
    void show_table(context& p_context);
};