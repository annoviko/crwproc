#include "state_search.h"

#include <iostream>
#include <unordered_map>

#include "core/console.h"
#include "core/proc_reader.h"

#include "command.h"
#include "intro_builder.h"


event state_search::operator()(context& p_context) {
    intro_builder::show(p_context, "Run search process, please wait...");

    std::size_t reported_progress = 0;
    proc_reader reader(p_context.get_proc_info(), p_context.get_filter());
    auto observer = [&reported_progress](std::size_t p_progress) {
        std::size_t bars_to_display = (p_progress - reported_progress) / 2;
        if (bars_to_display != 0) {
            reported_progress = p_progress;
            std::cout << std::string(bars_to_display, char(219));
        }
    };

    reader.subscribe(observer);
    std::cout << "Progress: ";

    if (p_context.get_found_values().empty()) {
        p_context.get_found_values() = reader.read_and_filter();
    }
    else {
        p_context.get_found_values() = reader.read_and_filter(p_context.get_found_values());
    }

    std::cout << std::endl;

    if (p_context.get_found_values().empty()) {
        console::warning_and_wait_key("Nothing has been found, please change filter.");
        return event_filter{};
    }

    std::cout << "Amount of found values: " << p_context.get_found_values().size() << "." << std::endl;

    return ask_next_action(p_context);
}


event state_search::ask_next_action(context& p_context) const {
    std::cout << "Please enter the command to continue: ";

    std::string user_input;
    std::cin >> user_input;

    event action = command::to_event(user_input);
    std::visit([&user_input, &p_context, &action](auto&& instance) {
        using EventType = std::decay_t<decltype(instance)>;
        if constexpr (std::is_same_v<EventType, event_error>) {
            console::error_and_wait_key("Error: unknown command is specified '" + user_input + "'.");
        }
        else if (std::is_same_v<EventType, event_set>) {

        }
    }, action);

    return action;
}
