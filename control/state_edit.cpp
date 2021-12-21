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
#include "core/string_split.h"
#include "core/string_utils.h"

#include "log/logging.h"

#include "asker.h"
#include "command.h"
#include "intro_builder.h"
#include "edit_table_reader.h"
#include "edit_table_writer.h"
#include "log_wrapper.h"


const state_edit::column_name_map state_edit::COLUMN_NAME_MAP = {
    { state_edit::column_element::number, "Nr" },
    { state_edit::column_element::address, "Address" },
    { state_edit::column_element::name, "Name" },
    { state_edit::column_element::type, "Type" },
    { state_edit::column_element::value, "Value" },
};


event state_edit::operator()(context& p_context) const {
    intro_builder::show(p_context, "User table to edit values:");
    show_table(p_context);

    return ask_next_action(p_context);
}


void state_edit::show_table(context& p_context) {
    for (auto& entry : p_context.get_user_table()) {
        entry.refresh(p_context.get_proc_info());
    }

    column_position_map position_map = get_column_position_map(p_context);
    column_names names = get_column_names(position_map);

    console_table view_table(p_context.get_user_table().size() + 1, names.size());
    view_table.set_column_names(names);

    for (std::size_t i = 0; i < p_context.get_user_table().size(); i++) {
        const edit_table_entry& entry = p_context.get_user_table().at(i);
        const std::string entry_value = entry.is_valid() ? entry.get_pointer().get_value().to_string(entry.get_type()) : INVALID_VALUE;

        std::stringstream stream;

        std::size_t row_number = i + 1;
        view_table.set_cell_content(row_number, position_map.at(column_element::number), std::to_string(i));

        stream << (void*)entry.get_pointer().get_address();
        view_table.set_cell_content(row_number, position_map.at(column_element::address), stream.str());
        view_table.set_cell_content(row_number, position_map.at(column_element::type), entry.get_type().to_string());
        view_table.set_cell_content(row_number, position_map.at(column_element::value), entry_value);

        const auto iter_position = position_map.find(column_element::name);
        if (iter_position != position_map.cend()) {
            view_table.set_cell_content(row_number, iter_position->second, entry.get_name());
        }
    }

    view_table.show();

    std::cout << std::endl;
}


bool state_edit::has_table_names(const context& p_context) {
    for (const auto& p_entry : p_context.get_user_table()) {
        if (!p_entry.get_name().empty()) {
            return true;
        }
    }

    return false;
}


state_edit::column_position_map state_edit::get_column_position_map(const context& p_context) {
    const bool has_names = has_table_names(p_context);

    column_position_map result;
    for (std::size_t i = 0, position = 0; i < static_cast<std::size_t>(column_element::last_element); ++i) {
        const column_element column = static_cast<column_element>(i);
        if (!has_names && (column == column_element::name)) {
            continue;
        }

        result.insert({ column, position++ });
    }

    return result;
}


state_edit::column_names state_edit::get_column_names(const state_edit::column_position_map& p_position) {
    column_names result;

    for (std::size_t i = 0; i < static_cast<std::size_t>(column_element::last_element); ++i) {
        const column_element column = static_cast<column_element>(i);
        const auto iter_position = p_position.find(column);
        if (iter_position != p_position.end()) {
            result.push_back(get_column_name(iter_position->first));
        }
    }

    return result;
}


std::string state_edit::get_column_name(const column_element p_element) {
    const auto iter = COLUMN_NAME_MAP.find(p_element);
    if (iter != COLUMN_NAME_MAP.cend()) {
        return iter->second;
    }

    return "invalid";
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
        else if (std::is_same_v<EventType, event_rename>) {
            handle_rename_event(p_context);
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


void state_edit::handle_rename_event(context& p_context) {
    const index_info info = asker::ask_index(p_context.get_user_table().size(), true);
    if (!info.is_valid()) {
        return;
    }

    std::string string_name;
    std::getline(std::cin, string_name);

    crwproc::string::utils::trim(string_name);

    std::vector<std::string> last_arguments = { };
    crwproc::string::split(string_name, std::back_inserter(last_arguments));

    if (last_arguments.empty() || (last_arguments.size() > 1)) {
        LOG_ERROR_WITH_WAIT_KEY_AND_RETURN("Error: variable name should be represented by a value without spaces.")
    }

    LOG_INFO("User input (new name for variable): '" << string_name << "'.")

    p_context.get_user_table().at(info.get_begin()).set_name(string_name);
}


std::ostream& operator<<(std::ostream& p_stream, const state_edit&) {
    p_stream << "state_edit";
    return p_stream;
}
