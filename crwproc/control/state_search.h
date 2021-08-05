#pragma once

#include "context.h"
#include "event.h"


class state_search {
public:
    event operator()(context& p_context);

private:
    void show_intro(const context& p_context) const;
};
