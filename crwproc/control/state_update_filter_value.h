#pragma once

#include "context.h"
#include "event.h"


class state_update_filter_value {
public:
    event operator()(context& p_context);

private:
    std::string update_value(const filter_equal& p_filter) const;
};