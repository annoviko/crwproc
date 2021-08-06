#pragma once

#include <functional>

#include "context.h"
#include "event.h"


class state_show_search_result {
private:
    using action_dict = std::unordered_map<std::string, std::function<event(context&)>>;
    using action_sequence = std::vector<action_dict::const_iterator>;

private:
    static const action_dict        AVAILABLE_ACTIONS;
    static const action_sequence    PROVIDED_ACTIONS;

public:
    event operator()(context& p_context);

private:
    void show_values(const context& p_context) const;

private:
    static action_dict get_available_actions();

    static action_sequence get_provided_actions(const state_show_search_result::action_dict& p_actions);
};