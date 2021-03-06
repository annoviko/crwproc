/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once


#include "context.h"
#include "event.h"
#include "state_base.h"


class state_help : public state_base {
public:
    event operator()(context& p_context) const;

private:
    static void show_help();

private:
    static std::size_t get_longest_command_name();

public:
    friend std::ostream& operator<<(std::ostream& p_stream, const state_help& p_state);
};