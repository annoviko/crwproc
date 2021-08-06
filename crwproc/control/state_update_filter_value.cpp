#include "state_update_filter_value.h"

#include <iostream>
#include <string>

#include "filter_reader_value.h"
#include "intro_builder.h"


event state_update_filter_value::operator()(context& p_context) {
    intro_builder::show(p_context, "Update value for the current filter.");

    const std::string value = update_value(p_context.get_filter());
    p_context.get_filter().set_value(value);

    return event_done{};
}


std::string state_update_filter_value::update_value(const filter_value& p_filter) const {
    std::string value = filter_reader_value::read(p_filter);
    if (value.empty()) {
        value = filter_reader_value::read(p_filter);
    }

    return value;
}
