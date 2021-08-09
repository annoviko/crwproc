#include "command.h"

#include <algorithm>
#include <cctype>
#include <iterator>


const std::unordered_map<std::string, event> command::COMMANDS = {
    { command::name::exit,              event_exit()     },
    { command::name::choose,            event_choose()   },
    { command::name::create_filter,     event_filter()   },
    { command::name::update_filter,     event_update()   },
    { command::name::add,               event_add()      },
    { command::name::show,              event_show()     },
    { command::name::edit,              event_edit()     },
    { command::name::set,               event_set()      },
    { command::name::refresh,           event_refresh()  }
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