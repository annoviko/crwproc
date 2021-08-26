#include "state_create_filter.h"

#include <iostream>
#include <limits>
#include <vector>

#include "core/console.h"

#include "asker.h"
#include "filter_reader_value.h"
#include "intro_builder.h"


const std::size_t state_create_filter::INVALID_SIZE = std::numeric_limits<std::size_t>::max();


event state_create_filter::operator()(context& p_context) {
    p_context.get_found_values().clear();

    intro_builder::show(p_context, "Create filter for the process.");
    ask_filter(p_context);
    return event_done{};
}


void state_create_filter::ask_filter(context& p_context) const {
    value::type type = ask_value_type();
    while (type == value::type::invalid) {
        type = ask_value_type();
    }

    std::size_t size = 0;
    if (type == value::type::integral) {
        size = ask_value_size();
        while (size == INVALID_SIZE) {
            size = ask_value_size();
        }
    }

    std::string value = filter_reader_value::read(p_context.get_filter());
    while (value.empty()) {
        value = filter_reader_value::read(p_context.get_filter());
    }

    p_context.get_filter() = filter_equal({ type, size, value });
}


value::type state_create_filter::ask_value_type() const {
    std::cout << "Select value type that is going to be searched in the process:" << std::endl;

    std::vector<decltype(value::STR_TYPE_DICT)::const_iterator> options;
    options.reserve(value::STR_TYPE_DICT.size());

    for (auto iter = value::STR_TYPE_DICT.cbegin(); iter != value::STR_TYPE_DICT.cend(); iter++) {
        console::set_foreground_color(color::blue, true);
        std::cout << " " << options.size();
        console::set_defaut_color();

        std::cout << " - " << iter->first << std::endl;
        options.push_back(iter);
    }

    std::cout << "Enter option number (0-" << options.size() - 1 << "): ";
    std::size_t index_option = asker::ask_index(options.size());

    if (index_option != asker::INVALID_INDEX) {
        std::cout << std::endl;
        return options[index_option]->second;
    }

    return value::type::invalid;
}


std::size_t state_create_filter::ask_value_size() const {
    std::cout << "Select value size that is going to be searched in the process:" << std::endl;

    std::vector<std::size_t> options = { 1, 2, 4, 8 };
    for (std::size_t i = 0; i < options.size(); i++) {
        console::set_foreground_color(color::blue, true);
        std::cout << " " << i;
        console::set_defaut_color();

        std::cout << " - " << options[i] << " byte(s)" << std::endl;
    }

    std::cout << "Enter option number (0-" << options.size() - 1 << "): ";
    std::size_t index_option = asker::ask_index(options.size());

    if (index_option != asker::INVALID_INDEX) {
        std::cout << std::endl;
        return options[index_option];
    }

    return INVALID_SIZE;
}