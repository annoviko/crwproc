#include "state_search.h"

#include <iostream>

event state_search::operator()(context& p_context) {
    std::cout << p_context << std::endl;
    std::cout << "Search state. Press any key to terminate the application." << std::endl;

    std::string ignore;
    std::cin >> ignore;

    return event_exit{};
}