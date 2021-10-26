/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once

#include "context.h"
#include "event.h"


class state_edit {
public:
    event operator()(context& p_context) const;

private:
    static void show_table(context& p_context);

    static event ask_next_action(context& p_context);

public:
    friend std::ostream& operator<<(std::ostream& p_stream, const state_edit& p_state);
};