/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once


#include <optional>
#include <set>
#include <string>

#include <windows.h>

#include "control.h"


namespace crwproc {

namespace console {


struct coutput {
    std::string content;
    std::optional<int> interrupt;
};


class reader {
private:
    static HANDLE m_handle;

private:
    coutput       m_output;
    std::set<int> m_interruptions;
    position      m_position;

public:
    virtual void register_interrupt(const int p_key);

    virtual coutput read_line();

    virtual void clear_output();

    virtual void clear_interrupt();

protected:
    bool handle_input(const INPUT_RECORD& p_input_record);

    bool handle_special_input(const INPUT_RECORD& p_input_record);

    bool handle_key(const INPUT_RECORD& p_input_record);

    void handle_backspace();
};


}

}
