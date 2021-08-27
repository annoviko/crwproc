#pragma once

#include "context.h"
#include "event.h"

#include <optional>

class state_create_filter {
private:
    static const std::size_t INVALID_SIZE;
    static const std::size_t INVALID_SIGN;

public:
    event operator()(context& p_context);

private:
    void ask_filter(context& p_context) const;
    value::type ask_value_type() const;
    std::size_t ask_value_size() const;
    std::optional<bool> ask_value_sign() const;
};