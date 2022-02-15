/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "state_show_search_result.h"

#include <iostream>
#include <iomanip>
#include <string>

#include "console/control.h"
#include "console/table.h"

#include "log/logging.h"

#include "asker.h"
#include "edit_table_entry.h"
#include "intro_builder.h"
#include "log_wrapper.h"


event state_show_search_result::operator()(context& p_context) {
    m_intro = intro_builder("Found values with the filter.");
    m_intro.show(p_context);

    show_values(p_context);

    return continue_until_event_is_not<event_add>([this, &p_context]() { return ask_next_action(p_context); });
}


void state_show_search_result::show_values(const context& p_context) const {
    const std::size_t amount_elements = p_context.get_found_values().get_amount_values();
    const std::size_t amount_rows = (p_context.get_found_values().get_amount_values() > MAX_VIEW_SIZE) ? MAX_VIEW_SIZE + 2 : p_context.get_found_values().get_amount_values() + 1;

    crwproc::console::table view_table(amount_rows, 2);
    view_table.set_column_names({ "Nr", "Address" });

    auto iter_cursor = p_context.get_found_values().begin();
    for (std::size_t i = 0; (i < amount_elements) && (i < MAX_VIEW_SIZE); i++, ++iter_cursor) {
        const proc_pointer& pointer = *iter_cursor;

        std::stringstream stream;
        stream << (void*)pointer.get_address();

        view_table.set_cell_content(i + 1, 0, std::to_string(i));
        view_table.set_cell_content(i + 1, 1, stream.str());
    }

    if (amount_elements > MAX_VIEW_SIZE) {
        view_table.set_cell_content(MAX_VIEW_SIZE + 1, 0, "...");
        view_table.set_cell_content(MAX_VIEW_SIZE + 1, 1, "...");
        view_table.show();

        std::cout << std::endl << "... (only the first " << MAX_VIEW_SIZE << " is displayed from " << p_context.get_found_values().get_amount_values() << ")" << std::endl;
    }
    else {
        view_table.show();
    }

    std::cout << std::endl;
}


event state_show_search_result::ask_next_action(context& p_context) {
    std::cout << "Please enter the command to continue: ";

    std::string user_input;
    std::cin >> user_input;

    LOG_INFO("User input (action): '" << user_input << "'.")

    event action = command::to_event(user_input);
    std::visit([this, &user_input, &p_context](auto&& action_instance) {
        using EventType = std::decay_t<decltype(action_instance)>;
        if constexpr (std::is_same_v<EventType, event_error>) {
            LOG_ERROR_WITH_WAIT_KEY("Error: unknown command is specified '" + user_input + "'.")
        }
        else if (std::is_same_v<EventType, event_add>) {
            const index_info info = asker::ask_index(p_context.get_found_values().get_amount_values(), false);
            if (!info.is_valid()) {
                return;
            }

            type_desc type = std::visit([](auto&& type_instance) { 
                return type_instance.get_type();
            }, p_context.get_filter());


            search_result::iterator iter_pointer = p_context.get_found_values().begin();
            std::advance(iter_pointer, info.get_begin());

            for (std::size_t i = info.get_begin(); i < info.get_end(); ++i, ++iter_pointer) {
                edit_table_entry entry(*iter_pointer, type);
                p_context.get_user_table().push_back(entry);
            }

            LOG_INFO("Values added to the edit table with indexes from '" << info.get_begin() << "' to '" 
                << info.get_end() << "' (edit table size: '" << p_context.get_user_table().size() << "').")

            m_intro.redraw(p_context);
        }
        else if (std::is_same_v<EventType, event_remove>) {
            const index_info info = asker::ask_index(p_context.get_found_values().get_amount_values(), false);
            if (!info.is_valid()) {
                return;
            }

            auto iter_begin_erase = p_context.get_found_values().begin();
            auto iter_end_erase = p_context.get_found_values().end();

            std::advance(iter_begin_erase, info.get_begin());
            std::advance(iter_end_erase, info.get_end());

            p_context.get_found_values().erase(iter_begin_erase, iter_end_erase);

            LOG_INFO("Remove values from the edit table with indexes from '" << info.get_begin() << "' to '" << info.get_end() << "'.")
        }
    }, action);

    return action;
}


std::ostream& operator<<(std::ostream& p_stream, const state_show_search_result&) {
    p_stream << "state_show_search_result";
    return p_stream;
}
