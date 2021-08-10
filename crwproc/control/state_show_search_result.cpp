#include "state_show_search_result.h"

#include <iostream>
#include <iomanip>
#include <string>

#include "core/console.h"

#include "asker.h"
#include "intro_builder.h"


event state_show_search_result::operator()(context& p_context) {
    intro_builder::show(p_context, "Found values with the filter.");
    show_values(p_context);

    return ask_next_action(p_context);
}


void state_show_search_result::show_values(const context& p_context) const {
    for (std::size_t i = 0; i < p_context.get_found_values().size(); i++) {
        const proc_pointer& pointer = p_context.get_found_values().at(i);

        std::cout << std::right << std::setw(4) << i << ") " <<
            "address: " << std::setw(10) << (void*)pointer.get_address() << "| " <<
            "type: " << std::setw(10) << value::type_to_string(pointer.get_value().get_type()) << "| " <<
            "value: " << std::setw(10) << pointer.get_value().get<std::string>() << std::endl;
    }

    std::cout << std::endl;
}


event state_show_search_result::ask_next_action(context& p_context) const {
    std::cout << "Please enter the command to continue: ";

    std::string user_input;
    std::cin >> user_input;

    event action = command::to_event(user_input);
    std::visit([this, &user_input, &p_context, &action](auto&& instance) {
        using EventType = std::decay_t<decltype(instance)>;
        if constexpr (std::is_same_v<EventType, event_error>) {
            console::error("Error: unknown command is specified '" + user_input + "'.", true);
        }
        else if (std::is_same_v<EventType, event_add>) {
            std::size_t index_value = asker::ask_index(p_context.get_found_values().size(), [&p_context](std::size_t p_index) {
                p_context.get_user_table().push_back(p_context.get_found_values().at(p_index));
            });
        }
        else if (std::is_same_v<EventType, event_remove>) {
            std::size_t index_value = asker::ask_index(p_context.get_found_values().size(), [&p_context](std::size_t p_index) {
                p_context.get_found_values().erase(p_context.get_found_values().begin() + p_index);
            });
        }
    }, action);

    return action;
}

