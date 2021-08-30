/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "state_address.h"

#include <iostream>
#include <iomanip>

#include "core/proc_reader.h"

#include "asker.h"
#include "intro_builder.h"


event state_address::operator()(context& p_context) {
    intro_builder::show(p_context, "Create a view value that is mapped on a specific address.");

    m_pointer = create_view();
    read(p_context.get_proc_info());
    show_value(p_context);

    event action = continue_until_event_is_not<event_refresh, event_add>([this, &p_context]() {
        show_value(p_context);
        return ask_next_action(p_context);
    });

    return action;
}


proc_pointer state_address::create_view() const {
    value variable = asker::ask_value();

    std::optional<std::uint64_t> address = asker::ask_address();
    while (!address.has_value()) {
        address = asker::ask_address();
    }

    return proc_pointer(address.value(), variable);
}


void state_address::read(const proc_info& p_info) {
    proc_reader reader(p_info, filter_none{ });
    m_pointer = reader.read({ m_pointer })[0];
}


void state_address::show_value(const context& p_context) const {
    console::clear();

    intro_builder::show(p_context, "View value that is mapped on a specific address.");

    std::cout << std::right <<
        "address: " << std::setw(10) << (void*)m_pointer.get_address() << "| " <<
        "type: "    << std::setw(10) << value::type_to_string(m_pointer.get_value().get_type()) << "| " <<
        "value: "   << std::setw(10) << m_pointer.get_value().get<std::string>() << std::endl << std::endl;
}


event state_address::ask_next_action(context& p_context) {
    event action = state_base::ask_next_action(p_context);

    std::visit([this, &p_context, &action](auto&& instance) {
        using EventType = std::decay_t<decltype(instance)>;

        if constexpr (std::is_same_v<EventType, event_add>) {
            p_context.get_user_table().push_back(m_pointer);
        }
    }, action);

    return action;
}