#pragma once

#include <functional>

#include "command.h"
#include "context.h"
#include "event.h"


class state_show_search_result {
private:
    using action_dict = std::unordered_map<std::string, std::function<event(context&)>>;
    using action_sequence = std::vector<action_dict::const_iterator>;

public:
    event operator()(context& p_context);

private:
    void show_values(const context& p_context) const;

    void ask_next_action(context& p_context) const;
};