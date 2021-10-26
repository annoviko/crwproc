/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "state_create_filter.h"

#include <iostream>
#include <limits>
#include <vector>

#include "core/console.h"

#include "log/logging.h"

#include "asker.h"
#include "intro_builder.h"


const std::unordered_map<state_create_filter::filter_type, std::string> state_create_filter::FILTER_TYPE_DICT = {
    { state_create_filter::filter_type::equal, "Filter Equal - to search exact value" },
    { state_create_filter::filter_type::range, "Filter Range - to search value in range (from 'A' to 'B')" },
    { state_create_filter::filter_type::more, "Filter More - to search value that are greater than before (x[t] > x[t-1])" },
    { state_create_filter::filter_type::less, "Filter Less - to search value that are smaller than before (x[t] < x[t-1])" }
};


const std::unordered_map<state_create_filter::filter_type, std::string> state_create_filter::FILTER_TYPE_SHORT_DICT = {
    { state_create_filter::filter_type::equal, "Filter Equal" },
    { state_create_filter::filter_type::range, "Filter Range)" },
    { state_create_filter::filter_type::more, "Filter More" },
    { state_create_filter::filter_type::less, "Filter Less" }
};


event state_create_filter::operator()(context& p_context) const {
    p_context.get_found_values().clear();

    intro_builder::show(p_context, "Create filter for the process.");
    create_filter(p_context);
    return event_done{};
}


void state_create_filter::create_filter(context& p_context) {
    filter_type type = ask_filter_type();
    while (type == filter_type::last_item) {
        type = ask_filter_type();
    }

    LOG_INFO("Filter type to create '" << type << "'.");

    switch (type) {
    case filter_type::equal:
        ask_filter<filter_equal>(p_context);
        break;

    case filter_type::range:
        ask_filter<filter_range>(p_context);
        break;

    case filter_type::more:
        ask_filter<filter_more>(p_context);
        break;

    case filter_type::less:
        ask_filter<filter_less>(p_context);
        break;

    default:
        throw std::logic_error("Unknown filter type '" + std::to_string(static_cast<std::size_t>(type)) + "' is detected by UI.");
    }
}


state_create_filter::filter_type state_create_filter::ask_filter_type() {
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


std::ostream& operator<<(std::ostream& p_stream, const state_create_filter&) {
    p_stream << "state_create_filter";
    return p_stream;
}


std::ostream& operator<<(std::ostream& p_stream, const state_create_filter::filter_type& p_state) {
    auto iter = state_create_filter::FILTER_TYPE_SHORT_DICT.find(p_state);
    if (iter == state_create_filter::FILTER_TYPE_SHORT_DICT.end()) {
        p_stream << "Unknown Filter";
    }
    else {
        p_stream << iter->second;
    }

    return p_stream;
}
