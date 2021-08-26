#pragma once

#include "context.h"
#include "event.h"


class state_create_filter {
private:
    static const std::size_t INVALID_SIZE;

public:
    event operator()(context& p_context);

private:
    void ask_filter(context& p_context) const;
    value::type ask_value_type() const;
    std::size_t ask_value_size() const;
};