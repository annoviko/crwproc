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

#include "log/logging.h"

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

    LOG_INFO("User input: '" << user_input << "'.");

    event action = command::to_event(user_input);

    std::visit([this, &user_input, &p_context, &action](auto&& instance) {
        using EventType = std::decay_t<decltype(instance)>;
        if constexpr (std::is_same_v<EventType, event_error>) {
            const std::string message = "Error: unknown command is specified '" + user_input + "'.";

            LOG_ERROR(message)
            console::error_and_wait_key(message);
        }
        else if (std::is_same_v<EventType, event_set>) {
            std::size_t index_value = asker::ask_index(p_context.get_user_table().size(), false);
            if (index_value == asker::INVALID_INDEX) {
                return;
            }

            if (index_value >= p_context.get_user_table().size()) {
                const std::string message = "Error: specified index '" + std::to_string(index_value) +
                    "' is out of range. The total amount of monitored values: " + std::to_string(p_context.get_user_table().size()) + ".";

                LOG_ERROR(message)
                console::error_and_wait_key(message);
                return;
            }

            std::string string_value;
            std::cin >> string_value;

            LOG_INFO("Set new value '" << string_value << "' to the target process.")

            edit_table_entry& entry = p_context.get_user_table().at(index_value);
            if (!entry.set_value(string_value, p_context.get_proc_info())) {
                const std::string message = "Error: impossible to write value '" + string_value + "' to the process.";

                LOG_ERROR(message)
                console::error_and_wait_key(message);

                return;
            }
        }
        else if (std::is_same_v<EventType, event_remove>) {
            std::size_t index_value = asker::ask_index(p_context.get_user_table().size(), [&p_context](std::size_t p_index) {
                p_context.get_user_table().erase(p_context.get_user_table().begin() + p_index);
            });

            LOG_INFO("Remove value with index '" << index_value << "' from the edit table.");
        }
    }, action);

    return action;
}


std::ostream& operator<<(std::ostream& p_stream, const state_edit& p_state) {
    p_stream << "state_edit";
    return p_stream;
}
