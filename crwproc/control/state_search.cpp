#include "state_search.h"

#include <iostream>
#include <unordered_map>

#include "core/proc_reader.h"

#include "intro_builder.h"


const state_search::action_dict state_search::AVAILABLE_ACTIONS = get_available_actions();

const state_search::action_sequence state_search::PROVIDED_ACTIONS = get_provided_actions(state_search::AVAILABLE_ACTIONS);


event state_search::operator()(context& p_context) {
    intro_builder::show(p_context, "Run search process, please wait...");

    proc_reader reader(p_context.get_proc_info(), p_context.get_filter());
    auto observer = [](std::uint64_t p_read_bytes, std::uint64_t p_size, std::uint64_t p_found_values) {
        std::cout << "Processed bytes: " << p_read_bytes << "/" << p_size << " (found values: " << p_found_values << ")." << std::endl;
    };

    reader.set_read_observer(observer);
    p_context.set_values(reader.read());

    if (p_context.get_found_values().empty()) {
        std::cout << "Nothing has been found, please change filter" << std::endl;
        return event_filter{};
    }

    std::cout << "Amount of found values: " << p_context.get_found_values().size() << "." << std::endl;

    return ask_next_action(p_context);
}


event state_search::ask_next_action(context& p_context) const {
    show_provided_actions();
    std::size_t option = ask_action_option();

    while (option >= PROVIDED_ACTIONS.size()) {
        std::cout << "Error: incorrect option '" << option << "' is chosen. Please choose option from range (0-" << PROVIDED_ACTIONS.size() - 1 << ")." << std::endl;
        show_provided_actions();
        option = ask_action_option();
    }

    return PROVIDED_ACTIONS[option]->second(p_context);
}


std::size_t state_search::ask_action_option() const {
    std::size_t option = 0;
    std::cin >> option;
    return option;
}


void state_search::show_provided_actions() const {
    std::cout << "Available actions:" << std::endl;
    for (std::size_t i = 0; i < PROVIDED_ACTIONS.size(); i++) {
        std::cout << i << " - " << PROVIDED_ACTIONS[i]->first << std::endl;
    }

    std::cout << std::endl << "Select next action (0-" << PROVIDED_ACTIONS.size() - 1 << "): ";
}


state_search::action_dict state_search::get_available_actions() {
    return
    {
        /* TODO: finish */
    };
}


state_search::action_sequence state_search::get_provided_actions(const state_search::action_dict& p_actions) {
    action_sequence result;
    result.reserve(p_actions.size());

    for (auto iter = p_actions.cbegin(); iter != p_actions.cend(); iter++) {
        result.push_back(iter);
    }

    return result;
}
