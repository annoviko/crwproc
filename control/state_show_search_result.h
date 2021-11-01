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


struct search_result_view_entry {
    proc_pointer_collection::iterator pointer_iterator;
    memory_block_sequence::iterator memory_block_iterator;
};


using search_result_view = std::vector<search_result_view_entry>;


class state_show_search_result {
private:
    static constexpr std::size_t MAX_VIEW_SIZE = 200;

private:
    using action_dict = std::unordered_map<std::string, std::function<event(context&)>>;
    using action_sequence = std::vector<action_dict::const_iterator>;

private:
    intro_builder m_intro;

    search_result_view m_view;

public:
    state_show_search_result() = default;

public:
    event operator()(context& p_context);

private:
    void show_values(const context& p_context) const;

    event ask_next_action(context& p_context);

    void build_view(context& p_context);

public:
    friend std::ostream& operator<<(std::ostream& p_stream, const state_show_search_result& p_state);
};