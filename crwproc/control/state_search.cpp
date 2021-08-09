#include "state_search.h"

#include <iostream>
#include <unordered_map>

#include "core/console.h"
#include "core/proc_reader.h"

#include "command.h"
#include "intro_builder.h"


event state_search::operator()(context& p_context) {
    intro_builder::show(p_context, "Run search process, please wait...");

    proc_reader reader(p_context.get_proc_info(), p_context.get_filter());
    auto observer = [](std::uint64_t p_read_bytes, std::uint64_t p_size, std::uint64_t p_found_values) {
        std::cout << "Processed bytes: " << p_read_bytes << "/" << p_size << " (found values: " << p_found_values << ")." << std::endl;
    };

    reader.set_read_observer(observer);

    if (p_context.get_found_values().empty()) {
        p_context.get_found_values() = reader.read_and_filter();
    }
    else {
        p_context.get_found_values() = reader.read_and_filter(p_context.get_found_values());
    }

    if (p_context.get_found_values().empty()) {
        console::warning("Nothing has been found, please change filter.", false);
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
            console::error("Error: unknown command is specified '" + user_input + "'.", true);
        }
        else if (std::is_same_v<EventType, event_set>) {

        }
    }, action);

    return action;
}
