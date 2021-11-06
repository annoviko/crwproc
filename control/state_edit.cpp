/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "state_edit.h"

#include <iomanip>
#include <iostream>

#include "core/console.h"
#include "core/console_table.h"
#include "core/proc_reader.h"
#include "core/proc_writer.h"

#include "log/logging.h"

#include "asker.h"
#include "command.h"
#include "intro_builder.h"
#include "edit_table_reader.h"
#include "edit_table_writer.h"
#include "log_wrapper.h"


event state_edit::operator()(context& p_context) const {
    intro_builder::show(p_context, "User table to edit values:");
    show_table(p_context);

    return ask_next_action(p_context);
}


void state_edit::show_table(context& p_context) {
    for (auto& entry : p_context.get_user_table()) {
        entry.refresh(p_context.get_proc_info());
    }

    console_table view_table(p_context.get_user_table().size() + 1, 4);
    view_table.set_column_names({ "Nr", "Address", "Type", "Value" });

    for (std::size_t i = 0; i < p_context.get_user_table().size(); i++) {
        const edit_table_entry& entry = p_context.get_user_table().at(i);

        std::stringstream stream;

        std::size_t row_number = i + 1;
        view_table.set_cell_content(row_number, 0, std::to_string(i));

        stream << (void*)entry.get_pointer().get_address();
        view_table.set_cell_content(row_number, 1, stream.str());
        view_table.set_cell_content(row_number, 2, entry.get_type().to_string());
        view_table.set_cell_content(row_number, 3, entry.get_pointer().get_value().to_string(entry.get_type()));
    }

    view_table.show();

    std::cout << std::endl;
}


event state_edit::ask_next_action(context& p_context) {
    std::cout << "Please enter the command to continue: ";

    std::string user_input;
    std::cin >> user_input;

    LOG_INFO("User input (next action): '" << user_input << "'.");

    event action = command::to_event(user_input);

    std::visit([&user_input, &p_context](auto&& instance) {
        using EventType = std::decay_t<decltype(instance)>;
        if constexpr (std::is_same_v<EventType, event_error>) {
            LOG_ERROR_WITH_WAIT_KEY_AND_RETURN("Error: unknown command is specified '" + user_input + "'.")
        }
        else if (std::is_same_v<EventType, event_set>) {
            handle_set_event(p_context);
        }
        else if (std::is_same_v<EventType, event_remove>) {
            handle_remove_event(p_context);
        }
        else if (std::is_same_v<EventType, event_save>) {
            handle_save_event(p_context);
        }
        else if (std::is_same_v<EventType, event_load>) {
            handle_load_event(p_context);
        }
        else if (std::is_same_v<EventType, event_revert>) {
            handle_revert_event(p_context);
        }
    }, action);

    return action;
}


void state_edit::handle_set_event(context& p_context) {
    index_info index_value = asker::ask_index(p_context.get_user_table().size(), false);
    if (!index_value.is_valid()) {
        return;
    }

    std::string string_value;
    std::cin >> string_value;

    LOG_INFO("Set new value '" << string_value << "' to the target process.")

    std::size_t amount_unaccessible = 0;

    p_context.get_user_table_change().clear();
    for (std::size_t i = index_value.get_begin(); i < index_value.get_end(); i++) {
        edit_table_entry& entry = p_context.get_user_table().at(i);

        p_context.get_user_table_change().add(entry);

        if (!entry.set_value(string_value, p_context.get_proc_info())) {
            amount_unaccessible++;
        }
    }

    if (amount_unaccessible > 0) {
        LOG_ERROR_WITH_WAIT_KEY_AND_RETURN("Error: impossible to set value '" + string_value + "' for '" + std::to_string(amount_unaccessible) + "' variables in the process.")
    }
}


void state_edit::handle_revert_event(context& p_context) {
    LOG_INFO("Revert changes that were done in the target process.")
    p_context.get_user_table_change().revert();
}


void state_edit::handle_remove_event(context& p_context) {
    const index_info info = asker::ask_index(p_context.get_user_table().size(), false);
    if (!info.is_valid()) {
        return;
    }

    p_context.get_user_table().erase(p_context.get_user_table().begin() + info.get_begin(), p_context.get_user_table().begin() + info.get_end());

    LOG_INFO("Remove values with indexes from '" << info.get_begin() << "' to '" << info.get_end() << "' from the edit table.")
}


void state_edit::handle_save_event(const context& p_context) {
    std::string filename;
    std::cin >> filename;

    LOG_INFO("User input (edit table filename): '" << filename << "'.")

    if (!edit_table_writer(filename).write(p_context.get_user_table())) {
        LOG_ERROR_WITH_WAIT_KEY_AND_RETURN("Error: impossible to save the edit table to file '" + filename + "'.")
    }
}


void state_edit::handle_load_event(context& p_context) {
    std::string filename;
    std::cin >> filename;

    LOG_INFO("User input (edit table filename): '" << filename << "'.")

    if (!edit_table_reader(filename).read(p_context.get_user_table())) {
        LOG_ERROR_WITH_WAIT_KEY_AND_RETURN("Error: impossible to load the edit table from file '" + filename + "'.")
    }
}


std::ostream& operator<<(std::ostream& p_stream, const state_edit&) {
    p_stream << "state_edit";
    return p_stream;
}
