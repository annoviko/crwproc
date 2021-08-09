#include "state_edit.h"

#include <iomanip>
#include <iostream>

#include "core/console.h"
#include "core/proc_reader.h"
#include "core/proc_writer.h"

#include "command.h"
#include "intro_builder.h"


event state_edit::operator()(context& p_context) {
    intro_builder::show(p_context, "User table to edit values:");
    show_table(p_context);

    return ask_next_action(p_context);
}


void state_edit::show_table(context& p_context) {
    proc_reader reader(p_context.get_proc_info(), p_context.get_filter());
    p_context.get_user_table() = reader.read(p_context.get_user_table());

    for (std::size_t i = 0; i < p_context.get_user_table().size(); i++) {
        proc_pointer& pointer = p_context.get_user_table().at(i);

        std::cout << std::right << std::setw(4) << i << ") " <<
            "address: " << std::setw(10) << (void*) pointer.get_address() << "| " << 
            "type: "    << std::setw(10) << value::type_to_string(pointer.get_value().get_type()) << "| " <<
            "value: "   << std::setw(10) << pointer.get_value().get_value() << std::endl;
    }

    std::cout << std::endl;
}


event state_edit::ask_next_action(context& p_context) const {
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
            std::size_t index_value = 0;
            std::cin >> index_value;

            if (index_value >= p_context.get_user_table().size()) {
                console::error("Error: specified index '" + std::to_string(index_value) + 
                    "' is out of range. The total amount of monitored values: " + std::to_string(p_context.get_user_table().size()) + ".", true);
                return;
            }

            std::string string_value;
            std::cin >> string_value;

            proc_pointer new_value = p_context.get_user_table().at(index_value);
            new_value.get_value().set_value(string_value);

            proc_writer writer(p_context.get_proc_info());
            if (!writer.write(new_value)) {
                console::error("Error: impossible to write value to the process.", true);
                return;
            }

            p_context.get_found_values().at(index_value) = new_value;
        }
    }, action);

    return action;
}