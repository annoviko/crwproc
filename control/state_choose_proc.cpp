/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "state_choose_proc.h"

#include <iomanip>
#include <iostream>
#include <string>
#include <type_traits>

#include "core/console.h"
#include "core/console_table.h"

#include "log/logging.h"

#include "command.h"
#include "intro_builder.h"
#include "log_wrapper.h"


event state_choose_proc::operator()(context& p_context) const {
    intro_builder::show(p_context, "Please, choose process by using its PID (Process ID).");

    const proc_collection procs = proc_table().get();
    show_procs(procs);

    const std::string input = ask_proc();
    LOG_INFO("User input '" << input << "'.")

    return process_user_input(input, procs, p_context);
}


void state_choose_proc::show_procs(const proc_collection& p_table) {
    console_table view_table(p_table.size() + 1, 2);
    view_table.set_column_names({ "PID", "Name" });
    view_table.get_col_property(1).alignment = console_table_cell_alignment::left;

    std::size_t row_number = 1;
    for (const auto& info : p_table) {
        view_table.set_cell_content(row_number, 0, std::to_string(info.second.pid()));
        view_table.set_cell_content(row_number, 1, info.second.name());

        row_number++;
    }

    view_table.show();
}


std::string state_choose_proc::ask_proc() {
    std::cout << "Please, enter PID or crwproc command: ";

    std::string user_input;
    std::cin >> user_input;

    LOG_INFO("User input (pid): '" << user_input << "'.")

    std::cout << std::endl;
    return user_input;
}


event state_choose_proc::process_user_input(const std::string& p_input, const proc_collection& p_table, context& p_context) {
    const std::size_t pid = try_get_pid(p_input);
    if (pid != proc_info::INVALID_PID) {
        return assign_pid(pid, p_table, p_context);
    }

    return process_user_command(p_input);
}


std::size_t state_choose_proc::try_get_pid(const std::string& p_input) {
    try {
        return std::stoul(p_input);
    }
    catch (...) {
        LOG_ERROR("Non-integer input '" << p_input << "' is provided as PID.")
        return proc_info::INVALID_PID;
    }
}


event state_choose_proc::assign_pid(const std::size_t p_pid, const proc_collection& p_table, context& p_context) {
    const auto iter = p_table.find(p_pid);
    if (iter == p_table.cend()) {
        LOG_ERROR_WITH_WAIT_KEY_AND_RETURN_VALUE("Error: process ID '" + std::to_string(p_pid) + "' does not exist.", event_error{ })
    }

    p_context.set_proc_info(iter->second);
    return event_done{};
}


event state_choose_proc::process_user_command(const std::string& p_command) {
    event event_to_handle = command::to_event(p_command);
    std::visit([&p_command](auto && instance) {
        using EventType = std::decay_t<decltype(instance)>;
        if constexpr (std::is_same_v<EventType, event_error>) {
            LOG_ERROR_WITH_WAIT_KEY("Error: unknown command is specified '" + p_command + "'.")
        }
    }, event_to_handle);

    return event_to_handle;
}


std::ostream& operator<<(std::ostream& p_stream, const state_choose_proc&) {
    p_stream << "state_choose_proc";
    return p_stream;
}
