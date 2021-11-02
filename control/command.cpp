/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "command.h"
#include "command_interrupt.h"

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
    { command::name::address,           event_address()  },
    { command::name::refresh,           event_refresh()  },
    { command::name::remove,            event_remove()   },
    { command::name::help,              event_help()     },
    { command::name::save,              event_save()     },
    { command::name::load,              event_load()     },
    { command::name::revert,            event_revert()   }
};


const std::unordered_map<std::string, std::string> command::DESCRIPTION = {
    { command::name::exit,              "Exit from the application." },
    { command::name::choose,            "Open a menu where process (application) can be chosen for processing." },
    { command::name::create_filter,     "Open a menu to create a new filter (table of variables is cleaned)." },
    { command::name::update_filter,     "Open a menu to update current filter." },
    { command::name::add,               "Add variable with a specific index from a current table to the edit table.\nSyntax: \\add <index>|all|1half|2half" },
    { command::name::show,              "Open a menu where found variables are displayed." },
    { command::name::edit,              "Open a menu where variables to edit are displayed." },
    { command::name::set,               "Set new value for a specific variable in the edit table.\nSyntax: \\set <index>|all|1half|2half <value>" },
    { command::name::address,           "Open a menu to read a value from the specific address." },
    { command::name::refresh,           "Refresh current view." },
    { command::name::remove,            "Remove a variable with a specific index from a table.\nSyntax: \\remove <index>|all|1half|2half" },
    { command::name::help,              "Show help information: existed commands with examples." },
    { command::name::save,              "Save the edit table to the specific file.\nSyntax: \\save <filename>" },
    { command::name::load,              "Load the edit table from the specific file.\nSyntax: \\load <filename>" },
    { command::name::revert,            "Revert changes introduced by \\set command." }
};


const std::unordered_map<std::string, std::vector<std::string>> command::EXAMPLES = {
    { command::name::add, {
            "\\add 0            - add variable with index 0 from the table of found variables to the edit table.",
            "\\add 20           - add variable with index 20 from the table of found variables to the edit table.",
            "\\add all          - add all variables from the table of found variables to the edit table."
        }
    },

    { command::name::set, {
            "\\set 0 99999      - set value 99999 to the variable with index 0 in the edit table.",
            "\\set all 100      - set value 100 to all variables in the edit table.",
            "\\set 1half 16     - set value 16 to the first half of the edit table.",
            "\\set 2half 64     - set value 64 to the second half of the edit table."
        }
    },

    { command::name::remove, {
            "\\remove 5         - remove variable with index 5 from a table.",
            "\\remove 15        - remove variable with index 15 from a table.",
            "\\remove 1half     - remove the first half of a table."
        }
    },

    { command::name::save, {
            "\\save proc_table.json - save the edit table to 'proc_table.json' file.",
        }
    },

    { command::name::load, {
            "\\load proc_table.json - load the edit table from 'proc_table.json' file.",
        }
    },
};


const command::description_dict& command::get_description() {
    return DESCRIPTION;
}


const command::example_dict& command::get_examples() {
    return EXAMPLES;
}


event command::to_event(const std::string& p_command) {
    std::string canonical_command;
    std::transform(p_command.begin(), p_command.end(), std::back_inserter(canonical_command), [](const char symbol) {
        return static_cast<char>(std::tolower(symbol));
    });

    const auto iter = COMMANDS.find(canonical_command);
    if (iter == COMMANDS.cend()) {
        return event_error();
    }

    return iter->second;
}


void command::throw_if_command(const std::string& p_user_input) {
    const event action = command::to_event(p_user_input);
    if (!is_event_type<event_error>(action)) {
        throw command_interrupt(action);
    }
}

