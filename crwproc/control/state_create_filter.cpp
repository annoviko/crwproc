#include "state_create_filter.h"

#include <iostream>
#include <limits>
#include <vector>

#include "filter_reader_value.h"
#include "intro_builder.h"


const std::size_t state_create_filter::INVALID_SIZE = std::numeric_limits<std::size_t>::max();


event state_create_filter::operator()(context& p_context) {
    intro_builder::show(p_context, "Create filter for the process.");
    ask_filter(p_context);
    return event_done{};
}


void state_create_filter::ask_filter(context& p_context) const {
    filter_value::type type = ask_filter_type();
    while (type == filter_value::type::invalid) {
        type = ask_filter_type();
    }

    std::size_t size = 0;
    if (type == filter_value::type::integral) {
        size = ask_value_size();
        while (size == INVALID_SIZE) {
            size = ask_value_size();
        }
    }

    std::string value = filter_reader_value::read(p_context.get_filter());
    if (value.empty()) {
        value = filter_reader_value::read(p_context.get_filter());
    }

    p_context.set_filter(filter_value(type, size, value));
}


filter_value::type state_create_filter::ask_filter_type() const {
    std::cout << "Select value type that is going to be searched in the process:" << std::endl;

    std::vector<decltype(filter_value::STR_TYPE_DICT)::const_iterator> options;
    options.reserve(filter_value::STR_TYPE_DICT.size());

    for (auto iter = filter_value::STR_TYPE_DICT.cbegin(); iter != filter_value::STR_TYPE_DICT.cend(); iter++) {
        std::cout << options.size() << " - " << iter->first << std::endl;
        options.push_back(iter);
    }

    std::cout << "Enter option number (0-" << options.size() - 1 << "): ";
    std::size_t index_option = std::numeric_limits<std::size_t>::max();
    std::cin >> index_option;   /* TODO: possibility to use command */

    if (index_option < options.size()) {
        std::cout << std::endl;
        return options[index_option]->second;
    }

    std::cout << "Error: invalid filter is specified (user input '" << index_option << "')." << std::endl << std::endl;
    return filter_value::type::invalid;
}


std::size_t state_create_filter::ask_value_size() const {
    std::cout << "Select value size that is going to be searched in the process:" << std::endl;

    std::vector<std::size_t> options = { 1, 2, 4, 8 };
    for (std::size_t i = 0; i < options.size(); i++) {
        std::cout << i << " - " << options[i] << " byte(s)" << std::endl;
    }

    std::cout << "Enter option number (0-" << options.size() - 1 << "): ";
    std::size_t index_option = std::numeric_limits<std::size_t>::max();
    std::cin >> index_option;   /* TODO: possibility to use command */

    if (index_option < options.size()) {
        std::cout << std::endl;
        return options[index_option];
    }

    std::cout << "Error: invalid filter is specified (user input '" << index_option << "')." << std::endl << std::endl;
    return INVALID_SIZE;
}
