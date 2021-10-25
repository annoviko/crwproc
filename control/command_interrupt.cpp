/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "command_interrupt.h"


command_interrupt::command_interrupt(const event& p_event) :
    m_event(p_event)
{ }


const char* command_interrupt::what() const noexcept {
    return WHAT_MESSAGE;
}


event command_interrupt::get() const {
    return m_event;
}
