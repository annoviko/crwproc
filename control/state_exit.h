/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once

#include "context.h"
#include "event.h"


class state_exit {
public:
    event operator()(context& p_context) const;

public:
    friend std::ostream& operator<<(std::ostream& p_stream, const state_exit& p_state);
};