/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once


#include <set>
#include <string>
#include <vector>

#include <windows.h>

#include "control.h"


namespace crwproc {

namespace console {


struct stdin_output {
    std::string content;
    bool interrupted = false;
};


class interruptible_reader {
private:
    using record_sequence = std::vector<INPUT_RECORD>;

private:
    static const record_sequence UNLOCK_SEQUENCE;
    static const std::set<int> TERMINATE_KEYS;

private:
    stdin_output  m_output;
    std::set<int> m_interruptions;

public:
    void register_interrupt(const int p_key);

    stdin_output read_line();

    void clear_interrupt();

private:
    void wait_termination();

    bool is_interrupted();

    bool is_key_pressed(const int p_key);

private:
    static record_sequence get_unlock_sequence();
};


}

}
