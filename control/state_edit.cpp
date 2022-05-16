/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "state_edit.h"

#include <iomanip>
#include <iostream>

#include "console/control.h"
#include "console/table.h"

#include "core/proc_reader.h"
#include "core/proc_writer.h"
#include "core/string_numeric.h"
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
    { state_edit::column_element::multiplier, "Multiplier" },
    { state_edit::column_element::value, "Value" },
};


const state_edit::optional_columns state_edit::OPTIONAL_COLUMNS = {
    state_edit::column_element::name,
    state_edit::column_element::multiplier
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

    crwproc::console::table view_table(p_context.get_user_table().size() + 1, names.size());
    view_table.set_column_names(names);

    for (std::size_t i = 0; i < p_context.get_user_table().size(); i++) {
        const edit_table_entry& entry = p_context.get_user_table().at(i);
        const std::string entry_value = entry.get_value();

        std::stringstream stream;

        std::size_t row_number = i + 1;
        view_table.set_cell_content(row_number, position_map.at(column_element::number), std::to_string(i));

        stream << (void*)entry.get_pointer().get_address();
        view_table.set_cell_content(row_number, position_map.at(column_element::address), stream.str());
        view_table.set_cell_content(row_number, position_map.at(column_element::type), entry.get_type().to_string());
        view_table.set_cell_content(row_number, position_map.at(column_element::value), entry_value);

        auto iter_position = position_map.find(column_element::name);
        if (iter_position != position_map.end()) {
            view_table.set_cell_content(row_number, iter_position->second, entry.get_name());
        }

        iter_position = position_map.find(column_element::multiplier);
        if ((iter_position != position_map.end()) && (entry.get_multiplier() != edit_table_entry::DEFAULT_MULTIPLIER)) {
            view_table.set_cell_content(row_number, iter_position->second, std::to_string(entry.get_multiplier()));
        }
    }

    view_table.show();

    std::cout << std::endl;
}


state_edit::optional_columns state_edit::get_table_optional_columns(const context& p_context) {
    optional_columns columns;

    for (const auto& p_entry : p_context.get_user_table()) {
        if (!p_entry.get_name().empty()) {
            columns.insert(column_element::name);
        }

        if (p_entry.get_multiplier() != edit_table_entry::DEFAULT_MULTIPLIER) {
            columns.insert(column_element::multiplier);
        }

        if (OPTIONAL_COLUMNS.size() == columns.size()) {
            break;
        }
    }

    return columns;
}


state_edit::column_position_map state_edit::get_column_position_map(const context& p_context) {
    const optional_columns has_optiona_columns = get_table_optional_columns(p_context);

    column_position_map result;
    for (std::size_t i = 0, position = 0; i < static_cast<std::size_t>(column_element::last_element); ++i) {
        const column_element column = static_cast<column_element>(i);
        if ((OPTIONAL_COLUMNS.count(column) != 0) && (has_optiona_columns.count(column) == 0)) {
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


index_info::user_instruction state_edit::get_index_user_instruction(const context& p_context) {
    index_info::user_instruction result = { };

    for (std::size_t i = 0; i < p_context.get_user_table().size(); i++) {
        const std::string& entry_name = p_context.get_user_table()[i].get_name();
        if (!entry_name.empty()) {
            result[entry_name] = i;
        }
    }

    return result;
}


std::string state_edit::get_single_value_from_stdin() {
    std::string string_value;
    std::getline(std::cin, string_value);

    crwproc::string::utils::trim(string_value);

    std::vector<std::string> last_arguments = { };
    crwproc::string::split(string_value, std::back_inserter(last_arguments));

    if (last_arguments.empty() || (last_arguments.size() > 1)) {
        LOG_ERROR_WITH_WAIT_KEY_AND_RETURN_VALUE("Error: value should be represented by a value without spaces.", std::string())
    }

    return string_value;
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
        else if (std::is_same_v<EventType, event_set_multiplier>) {
            handle_set_multiplier(p_context);
        }
    }, action);

    return action;
}


void state_edit::handle_set_event(context& p_context) {
    const auto user_index_map = get_index_user_instruction(p_context);
    index_info index_value = asker::ask_index(p_context.get_user_table().size(), false, user_index_map);
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
    const auto index_user_map = get_index_user_instruction(p_context);
    const index_info info = asker::ask_index(p_context.get_user_table().size(), false, index_user_map);
    if (!info.is_valid()) {
        return;
    }

    for (std::size_t i = info.get_begin(); i < info.get_end(); i++) {
        const std::string& entry_name = p_context.get_user_table()[i].get_name();
        if (entry_name.empty()) {
            continue;
        }

        auto& active_names = p_context.get_user_table_active_names();
        auto iter = active_names.find(entry_name);
        if (iter != active_names.cend()) {
            active_names.erase(iter);
        }
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
    const auto index_user_map = get_index_user_instruction(p_context);
    const index_info info = asker::ask_index(p_context.get_user_table().size(), true, index_user_map);
    if (!info.is_valid()) {
        return;
    }

    std::string string_name = get_single_value_from_stdin();
    if (string_name.empty()) {
        return;
    }

    LOG_INFO("User input (new name for variable): '" << string_name << "'.")

    auto& table_active_names = p_context.get_user_table_active_names();
    if (table_active_names.count(string_name) != 0) {
        LOG_ERROR_WITH_WAIT_KEY_AND_RETURN("Error: variable name should be unique.")
    }

    auto& entry = p_context.get_user_table().at(info.get_begin());
    if (!entry.get_name().empty()) {
        LOG_INFO("Remove name '" + entry.get_name() + "' from table active names.")
        table_active_names.erase(entry.get_name());
    }

    LOG_INFO("Rename variable '" + entry.get_name() + "' to '" + string_name + "'.")

    entry.set_name(string_name);
    table_active_names.insert(string_name);
}


void state_edit::handle_set_multiplier(context& p_context) {
    const auto index_user_map = get_index_user_instruction(p_context);
    const index_info info = asker::ask_index(p_context.get_user_table().size(), false, index_user_map);
    if (!info.is_valid()) {
        return;
    }

    std::string string_multiplier = get_single_value_from_stdin();
    if (string_multiplier.empty()) {
        return;
    }

    LOG_INFO("User input (new multiplier for variable): '" << string_multiplier << "'.")

    const auto optional_integer = crwproc::string::stonum<std::int64_t>(string_multiplier);
    if (!optional_integer.has_value()) {
        LOG_ERROR_WITH_WAIT_KEY_AND_RETURN("Error: multiplier should be integer.")
    }

    auto& entry = p_context.get_user_table().at(info.get_begin());
    entry.set_multiplier(optional_integer.value());
}


std::ostream& operator<<(std::ostream& p_stream, const state_edit&) {
    p_stream << "state_edit";
    return p_stream;
}
