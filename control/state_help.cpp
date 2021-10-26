/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "state_help.h"

#include <iomanip>

#include "core/console.h"
#include "command.h"


event state_help::operator()(context& p_context) const {
    show_help();
    return ask_next_action(p_context);
}


void state_help::show_help() {
    static const std::size_t longest_name = get_longest_command_name();
    for (auto& pair : command::get_description()) {
        console::set_foreground_color(color::blue, true);
        std::cout << std::left << std::setw(longest_name + 3) << pair.first;
        console::set_defaut_color();

        std::cout << " - " << pair.second << std::endl;

        auto iter_example = command::get_examples().find(pair.first);
        if (iter_example != command::get_examples().cend()) {
            std::cout << "Examples:" << std::endl;
            for (const auto& example : iter_example->second) {
                std::cout << " - " << example << std::endl;
            }

            std::cout << std::endl;
        }
    }

    std::cout << std::endl;
}


std::size_t state_help::get_longest_command_name() {
    std::size_t result = 0;
    for (const auto& pair : command::get_description()) {
        result = std::max(pair.first.size(), result);
    }

    return result;
}


std::ostream& operator<<(std::ostream& p_stream, const state_help&) {
    p_stream << "state_help";
    return p_stream;
}
