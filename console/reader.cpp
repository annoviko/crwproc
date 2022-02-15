/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "reader.h"

#include <iostream>


namespace crwproc {

namespace console {


HANDLE reader::m_handle = GetStdHandle(STD_INPUT_HANDLE);


void reader::register_interrupt(const int p_key) {
    m_interruptions.insert(p_key);
}


coutput reader::read_line() {
    m_position = control::get_cursor_position();

    bool finish = false;
    while(!finish) {
        DWORD number_records = 0;
        INPUT_RECORD input_record;

        ReadConsoleInput(m_handle, &input_record, 1, &number_records);

        finish = handle_key(input_record);
    }

    return m_output;
}


void reader::clear_output() {
    m_output.content.clear();
    m_output.interrupt.reset();
}


void reader::clear_interrupt() {
    m_interruptions.clear();
}


bool reader::handle_input(const INPUT_RECORD& p_input_record) {
    const char key = p_input_record.Event.KeyEvent.uChar.AsciiChar;

    switch(key) {
    case '\n':
    case '\r':
        return true;

    default:
        m_output.content += static_cast<char>(key);
        std::cout << key;
    }

    return false;
}


void reader::handle_backspace() {
    position cursor = control::get_cursor_position();

    if (cursor < m_position) {
        cursor = m_position;
    }
    else {
        std::cout << " ";
        m_output.content.pop_back();
    }

    control::set_cursor_position(cursor);
}


bool reader::handle_special_input(const INPUT_RECORD& p_input_record) {
    const int key = p_input_record.Event.KeyEvent.wVirtualKeyCode;

    if (key == VK_BACK) {
        handle_backspace();
        return false;
    }

    const auto iter = m_interruptions.find(key);
    if (iter != m_interruptions.cend()) {
        m_output.interrupt = key;
        return true;
    }

    return false;
}


bool reader::handle_key(const INPUT_RECORD& p_input_record) {
    if (p_input_record.EventType != KEY_EVENT) {
        return false;
    }

    if (p_input_record.Event.KeyEvent.bKeyDown) {
        return handle_input(p_input_record);
    }

    return handle_special_input(p_input_record);
}


}

}