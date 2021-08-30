/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "state_create_filter.h"

#include <iostream>
#include <limits>
#include <vector>

#include "core/console.h"

#include "asker.h"
#include "intro_builder.h"


const std::size_t state_create_filter::INVALID_SIZE = std::numeric_limits<std::size_t>::max();
const std::size_t state_create_filter::INVALID_SIGN = std::numeric_limits<std::size_t>::max();

const std::unordered_map<state_create_filter::filter_type, std::string> state_create_filter::FILTER_TYPE_DICT = {
    { state_create_filter::filter_type::equal, "Filter Equal - to search exact value" },
    { state_create_filter::filter_type::range, "Filter Range - to search value in range (from 'A' to 'B')" }
};


event state_create_filter::operator()(context& p_context) {
    p_context.get_found_values().clear();

    intro_builder::show(p_context, "Create filter for the process.");
    create_filter(p_context);
    return event_done{};
}


void state_create_filter::create_filter(context& p_context) const {
    filter_type type = ask_filter_type();
    while (type == filter_type::last_item) {
        type = ask_filter_type();
    }

    switch (type) {
    case filter_type::equal:
        ask_filter<filter_equal>(p_context);
        break;

    case filter_type::range:
        ask_filter<filter_range>(p_context);
        break;

    default:
        throw std::logic_error("Unknown filter type '" + std::to_string(static_cast<std::size_t>(type)) + "' is detected by UI.");
    }
}


state_create_filter::filter_type state_create_filter::ask_filter_type() const {
    static const std::size_t amount_filters = static_cast<std::size_t>(filter_type::last_item);

    std::cout << "Select filter type that is going to be used for searching:" << std::endl;

    for (std::size_t i = 0; i < amount_filters; i++) {
        console::set_foreground_color(color::blue, true);
        std::cout << " " << i;
        console::set_defaut_color();

        auto iter = FILTER_TYPE_DICT.find(static_cast<filter_type>(i));
        if (iter == FILTER_TYPE_DICT.end()) {
            throw std::logic_error("Unknown filter type '" + std::to_string(i) + "' is detected by UI.");
        }

        std::cout << " - " << iter->second << std::endl;
    }

    std::cout << "Enter option number (0-" << amount_filters - 1 << "): ";
    std::size_t index_option = asker::ask_index(amount_filters);

    if (index_option != asker::INVALID_INDEX) {
        std::cout << std::endl;
        return static_cast<filter_type>(index_option);
    }

    return filter_type::last_item;
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

        std::cout << " - " << options[i] << " byte" << (options[i] == 1 ? "" : "s") << std::endl;
    }

    std::cout << "Enter option number (0-" << options.size() - 1 << "): ";
    std::size_t index_option = asker::ask_index(options.size());

    if (index_option != asker::INVALID_INDEX) {
        std::cout << std::endl;
        return options[index_option];
    }

    return INVALID_SIZE;
}


std::optional<bool> state_create_filter::ask_value_sign() const {
    std::cout << "Select integral value type:" << std::endl;

    std::vector<std::string> options = { "unsigned", "signed" };
    for (std::size_t i = 0; i < options.size(); i++) {
        console::set_foreground_color(color::blue, true);
        std::cout << " " << i;
        console::set_defaut_color();

        std::cout << " - " << options[i] << std::endl;
    }


    std::cout << "Enter option number (0-" << options.size() - 1 << "): ";
    std::size_t index_option = asker::ask_index(options.size());

    if (index_option != asker::INVALID_INDEX) {
        std::cout << std::endl;
        return (index_option == 0) ? false : true;
    }

    return { };
}
