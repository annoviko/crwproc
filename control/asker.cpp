#include "asker.h"

#include <iostream>
#include <string>

#include "core/console.h"


std::size_t asker::ask_index(const std::size_t p_limit, const action_index& p_action) {
    std::size_t index_value = 0;
    std::cin >> index_value;

    if (std::cin.fail()) {
        console::error_and_wait_key("Error: integer value is expected.");
        std::cin.clear();
        std::cin.ignore(256, '\n');
        return INVALID_INDEX;
    }

    if (index_value >= p_limit) {
        console::error_and_wait_key("Error: specified index '" + std::to_string(index_value) + "' is out of range (0-" + std::to_string(p_limit) + ").");
        return INVALID_INDEX;
    }

    if (p_action) {
        p_action(index_value);
    }
    
    return index_value;
}