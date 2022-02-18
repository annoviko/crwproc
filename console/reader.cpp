/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "reader.h"

#include <algorithm>
#include <iostream>
#include <thread>

#include <windows.h>


namespace crwproc {

namespace console {


const interruptible_reader::record_sequence interruptible_reader::UNLOCK_SEQUENCE = get_unlock_sequence();

const std::set<int> interruptible_reader::TERMINATE_KEYS = { '\n' };


void interruptible_reader::register_interrupt(const int p_key) {
    m_interruptions.insert(p_key);
}


stdin_output interruptible_reader::read_line() {
    std::thread thread_reader([this]() {
        std::string line;
        std::cin >> line;

        if (!m_output.interrupted) {
            m_output.content = std::move(line);
        }

        std::cin.get();
    });

    if (!m_interruptions.empty()) {
        wait_termination();
    }

    thread_reader.join();

    return m_output;
}


void interruptible_reader::wait_termination() {
    bool termination_flag = false;
    while(!termination_flag) {
        termination_flag = is_interrupted() || !m_output.content.empty();
    }
}


bool interruptible_reader::is_interrupted() {
    return std::any_of(m_interruptions.cbegin(), m_interruptions.cend(), [this](int p_key) {
        return is_key_pressed(p_key);
    });
}


bool interruptible_reader::is_key_pressed(const int p_key) {
    HANDLE stdin_handle = GetStdHandle(STD_INPUT_HANDLE);

    const bool key_pressed = (GetAsyncKeyState(p_key) & (1 << 16));
    if (key_pressed) {
        m_output.interrupted = true;

        DWORD event_written = 0;
        WriteConsoleInput(stdin_handle, UNLOCK_SEQUENCE.data(), static_cast<DWORD>(UNLOCK_SEQUENCE.size()), &event_written);

        return true;
    }

    return false;
}


void interruptible_reader::clear_interrupt() {
    m_interruptions.clear();
}


interruptible_reader::record_sequence interruptible_reader::get_unlock_sequence() {
    static const std::vector<std::size_t> unlock_keys = { 'A', VK_RETURN };

    record_sequence unlock_sequence(unlock_keys.size());
    for (std::size_t i = 0; i < unlock_keys.size(); i++) {
        unlock_sequence[i].EventType = KEY_EVENT;
        unlock_sequence[i].Event.KeyEvent.bKeyDown = TRUE;
        unlock_sequence[i].Event.KeyEvent.dwControlKeyState = 0;
        unlock_sequence[i].Event.KeyEvent.uChar.UnicodeChar = static_cast<WCHAR>(unlock_keys[i]);
        unlock_sequence[i].Event.KeyEvent.wRepeatCount = 1;
        unlock_sequence[i].Event.KeyEvent.wVirtualKeyCode = static_cast<WORD>(unlock_keys[i]);
        unlock_sequence[i].Event.KeyEvent.wVirtualScanCode = static_cast<WORD>(MapVirtualKey(static_cast<UINT>(unlock_keys[i]), MAPVK_VK_TO_VSC));
    }

    return unlock_sequence;
}


}

}