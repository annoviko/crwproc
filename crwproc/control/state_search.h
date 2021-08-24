#pragma once

#include <functional>
#include <vector>
#include <unordered_map>

#include "context.h"
#include "event.h"
#include "state_base.h"


class state_search : public state_base {
public:
    event operator()(context& p_context);
};
