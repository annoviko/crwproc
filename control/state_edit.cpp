/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "state_edit.h"

#include <iomanip>
#include <iostream>

#include "core/console.h"
#include "core/proc_reader.h"
#include "core/proc_writer.h"

#include "asker.h"
#include "command.h"
#include "intro_builder.h"


event state_edit::operator()(context& p_context) {
    intro_builder::show(p_context, "User table to edit values:");
    show_table(p_context);

    return ask_next_action(p_context);
}


void state_edit::show_table(context& p_context) {
    std::visit([&p_context]() {
        for (auto& entry : p_context.get_user_table()) {
            entry.refresh(p_context.get_proc_info());
        }
    });

    for (std::size_t i = 0; i < p_context.get_user_table().size(); i++) {
        const edit_table_entry& entry = p_context.get_user_table().at(i);

        std::cout << std::right << std::setw(4) << i << ") " <<
            "address: " << std::setw(10) << (void*)entry.get_pointer().get_address() << " | " <<
            "type: "    << std::setw(10) << entry.get_type() << " | " <<
            "value: "   << std::setw(10) << entry.get_pointer().get_value().to_string(entry.get_type()) << std::endl;
    }

    std::cout << std::endl;
}


event state_edit::ask_next_action(context& p_context) {
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
            std::size_t index_value = 0;
            std::cin >> index_value;

            if (index_value >= p_context.get_user_table().size()) {
                console::error_and_wait_key("Error: specified index '" + std::to_string(index_value) +
                    "' is out of range. The total amount of monitored values: " + std::to_string(p_context.get_user_table().size()) + ".");
                return;
            }

            std::string string_value;
            std::cin >> string_value;

            edit_table_entry& entry = p_context.get_user_table().at(index_value);
            if (!entry.set_value(string_value, p_context.get_proc_info())) {
                console::error_and_wait_key("Error: impossible to write value to the process.");
                return;
            }
        }
        else if (std::is_same_v<EventType, event_remove>) {
            std::size_t index_value = asker::ask_index(p_context.get_user_table().size(), [&p_context](std::size_t p_index) {
                p_context.get_user_table().erase(p_context.get_user_table().begin() + p_index);
            });
        }
    }, action);

    return action;
}