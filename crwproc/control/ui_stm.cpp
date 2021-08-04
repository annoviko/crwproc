#include "ui_stm.h"

#include "transitions.h"


void ui_stm::handle_state() {
    m_event = std::visit([this](auto& p_state) {
        return p_state(this->m_context);
    }, m_state);

    m_state = std::visit(transitions{}, m_state, m_event);
}