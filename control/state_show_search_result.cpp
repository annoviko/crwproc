/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "state_show_search_result.h"

#include <iostream>
#include <iomanip>
#include <string>

#include "core/console.h"

#include "asker.h"
#include "edit_table_entry.h"
#include "intro_builder.h"


event state_show_search_result::operator()(context& p_context) {
    m_intro = intro_builder("Found values with the filter.");
    m_intro.show(p_context);

    build_view(p_context);
    show_values(p_context);

    return continue_until_event_is_not<event_add>([this, &p_context]() { return ask_next_action(p_context); });
}


void state_show_search_result::show_values(const context& p_context) const {
    for (std::size_t i = 0; i < m_view.size(); i++) {
        const proc_pointer& pointer = *(m_view[i].pointer_iterator);

        std::cout << std::right << std::setw(4) << i << ") " <<
            "address: " << std::setw(10) << (void*)pointer.get_address() 
            << std::endl;
    }

    if (!m_is_complete_view) {
        std::cout << "... (only the first " << m_view.size() << " is displayed from " << p_context.get_found_values().get_amount_values() << ")" << std::endl;
    }

    std::cout << std::endl;
}


event state_show_search_result::ask_next_action(context& p_context) {
    std::cout << "Please enter the command to continue: ";

    std::string user_input;
    std::cin >> user_input;

    event action = command::to_event(user_input);
    std::visit([this, &user_input, &p_context, &action](auto&& instance) {
        using EventType = std::decay_t<decltype(instance)>;
        if constexpr (std::is_same_v<EventType, event_error>) {
            console::error_and_wait_key("Error: unknown command is specified '" + user_input + "'.");
        }
        else if (std::is_same_v<EventType, event_add>) {
            std::size_t index_value = asker::ask_index(m_view.size());
            if (index_value == asker::INVALID_INDEX) { 
                return;
            }

            type_desc type = std::visit([](auto&& instance) { 
                return instance.get_type(); 
            }, p_context.get_filter());

            edit_table_entry entry(*(m_view[index_value].pointer_iterator), type);
            p_context.get_user_table().push_back(entry);

            m_intro.redraw(p_context);
        }
        else if (std::is_same_v<EventType, event_remove>) {
            std::size_t index_value = asker::ask_index(m_view.size(), [this, &p_context](std::size_t p_index) {
                auto& iter_entry = m_view[p_index];
                iter_entry.memory_block_iterator->get_values().erase(iter_entry.pointer_iterator);

                m_view.erase(m_view.begin() + p_index);
            });
        }
    }, action);

    return action;
}


void state_show_search_result::build_view(context& p_context) {
    auto& blocks = p_context.get_found_values().get_memory_blocks();
    for (auto iter_block = blocks.begin(); iter_block != blocks.end(); iter_block++) {
        proc_pointer_collection& values = iter_block->get_values();

        for (auto iter_pointer = values.begin(); iter_pointer != values.end(); iter_pointer++) {
            if (m_view.size() >= MAX_VIEW_SIZE) {
                m_is_complete_view = false;
                return;
            }

            m_view.push_back({ iter_pointer, iter_block });
        }
    }

    m_is_complete_view = true;
}

