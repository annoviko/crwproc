/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "state_base.h"

#include "console/control.h"

#include "log/logging.h"

#include "command.h"
#include "log_wrapper.h"


event state_base::ask_next_action(const context&) {
    std::cout << "Please enter the command to continue: ";

    std::string user_input;
    std::cin >> user_input;

    LOG_INFO("User input (next action): '" << user_input << "'.")

    event action = command::to_event(user_input);
    std::visit([&user_input](auto&& instance) {
        using EventType = std::decay_t<decltype(instance)>;
        if constexpr (std::is_same_v<EventType, event_error>) {
            LOG_ERROR_WITH_WAIT_KEY("Error: unknown command is specified '" + user_input + "'.")
        }
    }, action);

    return action;
}
