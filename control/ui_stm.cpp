/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "ui_stm.h"

#include "command_interrupt.h"

#include "core/console.h"
#include "log/logging.h"

#include "transitions.h"


void ui_stm::handle_state() {
    try {
        m_event = std::visit([this](auto& p_state) {
            return p_state(this->m_context);
        }, m_state);

        LOG_INFO("State '" << m_state << "' produced event '" << m_event << "'.")
    }
    catch (command_interrupt& p_interruption) {
        LOG_INFO("Interruption event '" << p_interruption.get() << "' was received.")
        m_event = p_interruption.get();
    }

    console::clear();

    m_state = std::visit(transitions{}, m_state, m_event);
    LOG_INFO("New state: '" << m_state << "' after transition '" << m_event << "'.")
}