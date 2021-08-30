/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once

#include <functional>

#include "command.h"
#include "context.h"
#include "event.h"
#include "intro_builder.h"


class state_show_search_result {
private:
    using action_dict = std::unordered_map<std::string, std::function<event(context&)>>;
    using action_sequence = std::vector<action_dict::const_iterator>;

private:
    intro_builder m_intro;

public:
    state_show_search_result() = default;

public:
    event operator()(context& p_context);

private:
    void show_values(const context& p_context) const;

    event ask_next_action(context& p_context) const;
};