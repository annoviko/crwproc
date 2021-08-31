/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "ui_stm.h"

#include "core/console.h"

#include "transitions.h"


void ui_stm::handle_state() {
    try {
        m_event = std::visit([this](auto& p_state) {
            return p_state(this->m_context);
        }, m_state);
    }
    catch (event& p_interruption) {
        m_event = p_interruption;
    }

    console::clear();

    m_state = std::visit(transitions{}, m_state, m_event);
}