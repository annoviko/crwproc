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

#include "log/logging.h"

#include "command.h"
#include "intro_builder.h"


event state_choose_proc::operator()(context& p_context) const {
    intro_builder::show(p_context, "Please, choose process by using its PID (Process ID).");

    const proc_collection procs = proc_table().get();
    show_procs(procs);

    const std::string input = ask_proc();
    LOG_INFO("User input '" << input << "'.")

    return process_user_input(input, procs, p_context);
}


void state_choose_proc::show_procs(const proc_collection& p_table) {
    static const std::string highlighter(40, '-');

    std::cout << highlighter << std::endl;
    for (const auto& info : p_table) {
        std::cout << std::left << std::setw(10) << info.second.pid() << " | " << info.second.name() << std::endl;
    }

    std::cout << highlighter << std::endl << std::endl;
}


std::string state_choose_proc::ask_proc() {
    std::cout << "Please, enter PID or crwproc command: ";

    std::string user_input;
    std::cin >> user_input;

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
        const std::string message = "Error: process ID '" + std::to_string(p_pid) + "' does not exist.";

        LOG_ERROR(message);
        console::error_and_wait_key(message);

        return event_error{};
    }

    p_context.set_proc_info(iter->second);
    return event_done{};
}


event state_choose_proc::process_user_command(const std::string& p_command) {
    event event_to_handle = command::to_event(p_command);
    std::visit([&p_command](auto && instance) {
        using EventType = std::decay_t<decltype(instance)>;
        if constexpr (std::is_same_v<EventType, event_error>) {
            const std::string message = "Error: unknown command is specified '" + p_command + "'.";

            LOG_ERROR(message);
            console::error_and_wait_key(message);
        }
    }, event_to_handle);

    return event_to_handle;
}


std::ostream& operator<<(std::ostream& p_stream, const state_choose_proc&) {
    p_stream << "state_choose_proc";
    return p_stream;
}
