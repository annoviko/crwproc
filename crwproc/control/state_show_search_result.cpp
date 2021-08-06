#include "state_show_search_result.h"

#include <iostream>
#include <iomanip>
#include <string>

#include "intro_builder.h"


event state_show_search_result::operator()(context& p_context) {
    intro_builder::show(p_context, "Found values with the filter.");
    show_values(p_context);


    return event_filter{};
}


void state_show_search_result::show_values(const context& p_context) const {
    for (std::size_t i = 0; i < p_context.get_found_values().size(); i++) {
        const proc_pointer& pointer = p_context.get_found_values().at(i);
        std::cout << std::right << std::setw(4) << i <<
            ") address: " << std::setw(10) << (void*)pointer.address <<
            ", type: " << std::setw(10) << filter_value::type_to_string(pointer.type) << 
            ", value: " << pointer.value << std::endl;
    }
}


state_show_search_result::action_dict state_show_search_result::get_available_actions() {
    return
    {
        { "Set new filter and restart.", [](context& p_context) {
                p_context.set_values({});
                return event_filter{};
            }
        },

        { "Set new filter and continue.", [](context&) {
                return event_filter{};
            }
        },

        { "Set new value for the filter and continue.", [](context&) {
                return event_update{};
            }
        },

        { "Display found values.", [](context&) {
                return event_show{};
            }
        }
    };
}


state_show_search_result::action_sequence state_show_search_result::get_provided_actions(const state_show_search_result::action_dict& p_actions) {

}