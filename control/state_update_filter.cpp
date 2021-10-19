/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "state_update_filter.h"

#include <iostream>
#include <string>

#include "log/logging.h"

#include "filter_reader_value.h"
#include "intro_builder.h"


event state_update_filter::operator()(context& p_context) {
    intro_builder::show(p_context, "Update the current filter.");
    LOG_INFO("Update the current filter.");

    if (filter_reader_value::read(p_context.get_filter())) {
        return event_done{};
    }

    return event_error{};
}


std::ostream& operator<<(std::ostream& p_stream, const state_update_filter& p_state) {
    p_stream << "state_update_filter";
    return p_stream;
}
