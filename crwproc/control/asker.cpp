#include "asker.h"

#include <iostream>
#include <string>

#include "core/console.h"


std::size_t asker::ask_index(const std::size_t p_limit, const action_index& p_action) {
    std::size_t index_value = 0;
    std::cin >> index_value;

    if (index_value >= p_limit) {
        console::error("Error: specified index '" + std::to_string(index_value) + "' is out of range.", true);
        return INVALID_INDEX;
    }

    if (p_action) {
        p_action(index_value);
    }
    
    return index_value;
}