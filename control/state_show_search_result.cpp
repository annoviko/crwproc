/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "state_show_search_result.h"

#include <iostream>
#include <iomanip>
#include <string>

#include "core/console.h"
#include "core/console_table.h"

#include "log/logging.h"

#include "asker.h"
#include "edit_table_entry.h"
#include "intro_builder.h"
#include "log_wrapper.h"


event state_show_search_result::operator()(context& p_context) {
    m_intro = intro_builder("Found values with the filter.");
    m_intro.show(p_context);

    build_view(p_context);
    show_values(p_context);

    return continue_until_event_is_not<event_add>([this, &p_context]() { return ask_next_action(p_context); });
}


void state_show_search_result::show_values(const context& p_context) const {
    const std::size_t amount_rows = (m_view.size() > MAX_VIEW_SIZE) ? MAX_VIEW_SIZE + 2 : m_view.size() + 1;

    console_table view_table(amount_rows, 2);
    view_table.set_column_names({ "Nr", "Address" });

    for (std::size_t i = 0; (i < m_view.size()) && (i < MAX_VIEW_SIZE); i++) {
        const proc_pointer& pointer = *(m_view[i].pointer_iterator);

        std::stringstream stream;
        stream << (void*)pointer.get_address();

        view_table.set_cell_content(i + 1, 0, std::to_string(i));
        view_table.set_cell_content(i + 1, 1, stream.str());
    }

    if (m_view.size() > MAX_VIEW_SIZE) {
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
            const index_info info = asker::ask_index(m_view.size(), false);
            if (!info.is_valid()) {
                return;
            }

            type_desc type = std::visit([](auto&& type_instance) { 
                return type_instance.get_type();
            }, p_context.get_filter());

            for (std::size_t i = info.get_begin(); i < info.get_end(); i++) {
                edit_table_entry entry(*(m_view[i].pointer_iterator), type);
                p_context.get_user_table().push_back(entry);
            }

            LOG_INFO("Values added to the edit table with indexes from '" << info.get_begin() << "' to '" 
                << info.get_end() << "' (edit table size: '" << p_context.get_user_table().size() << "').")

            m_intro.redraw(p_context);
        }
        else if (std::is_same_v<EventType, event_remove>) {
            const index_info info = asker::ask_index(m_view.size(), false);
            if (!info.is_valid()) {
                return;
            }

            for (std::size_t i = info.get_begin(); i < info.get_end(); i++) {
                auto& iter_entry = m_view[i];
                iter_entry.memory_block_iterator->get_values().erase(iter_entry.pointer_iterator);
            }

            for (std::size_t i = info.get_begin(); i < info.get_end(); i++) {
                m_view.erase(m_view.begin() + i);
            }

            LOG_INFO("Remove values from the edit table with indexes from '" << info.get_begin() << "' to '" << info.get_end() << "'.")
        }
    }, action);

    return action;
}


void state_show_search_result::build_view(context& p_context) {
    auto& blocks = p_context.get_found_values().get_memory_blocks();
    for (auto iter_block = blocks.begin(); iter_block != blocks.end(); iter_block++) {
        proc_pointer_collection& values = iter_block->get_values();

        for (auto iter_pointer = values.begin(); iter_pointer != values.end(); iter_pointer++) {
            m_view.push_back({ iter_pointer, iter_block });
        }
    }
}


std::ostream& operator<<(std::ostream& p_stream, const state_show_search_result&) {
    p_stream << "state_show_search_result";
    return p_stream;
}
