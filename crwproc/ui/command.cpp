#include "command.h"

#include <algorithm>
#include <cctype>
#include <iterator>


const std::unordered_map<std::string, event> command::COMMANDS = {
    { "\\e",        event_exit() },
    { "\\exit",     event_exit() },
    { "\\c",        event_choose() },
    { "\\choose",   event_choose(), }
};


event command::to_event(const std::string& p_command) {
    std::string canonical_command;
    std::transform(p_command.begin(), p_command.end(), std::back_inserter(canonical_command), [](const char symbol) {
        return std::tolower(symbol);
    });

    const auto iter = COMMANDS.find(canonical_command);
    if (iter == COMMANDS.cend()) {
        return event_error();
    }

    return iter->second;
}