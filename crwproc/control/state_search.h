#pragma once

#include <functional>
#include <vector>
#include <unordered_map>

#include "context.h"
#include "event.h"


class state_search {
private:
    using action_dict = std::unordered_map<std::string, std::function<event(context&)>>;
    using action_sequence = std::vector<action_dict::const_iterator>;

private:
    static const action_dict        AVAILABLE_ACTIONS;
    static const action_sequence    PROVIDED_ACTIONS;

public:
    event operator()(context& p_context);

private:
    event ask_next_action(context& p_context) const;

    std::size_t ask_action_option() const;

    void show_provided_actions() const;

private:
    static action_dict get_available_actions();

    static action_sequence get_provided_actions(const state_search::action_dict& p_actions);
};
