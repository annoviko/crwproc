#pragma once


#include "context.h"
#include "event.h"
#include "state_base.h"


class state_help : public state_base {
private:
    static std::size_t LONGEST_COMMAND_NAME;

public:
    event operator()(context& p_context);

private:
    void show_help();

private:
    static std::size_t get_longest_command_name();
};