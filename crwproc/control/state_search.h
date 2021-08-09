#pragma once

#include <functional>
#include <vector>
#include <unordered_map>

#include "context.h"
#include "event.h"


class state_search {
public:
    event operator()(context& p_context);

private:
    event ask_next_action(context& p_context) const;
};
