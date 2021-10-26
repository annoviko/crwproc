/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once

#include "context.h"
#include "event.h"

#include "core/proc_table.h"


class state_choose_proc {
public:
    event operator()(context& p_context) const;

private:
    static void show_procs(const proc_collection& p_table);
    static std::string ask_proc();

    static event process_user_input(const std::string& p_input, const proc_collection& p_table, context& p_context);
    static std::size_t try_get_pid(const std::string& p_input);
    static event assign_pid(const std::size_t p_pid, const proc_collection& p_table, context& p_context);
    static event process_user_command(const std::string& p_command);

public:
    friend std::ostream& operator<<(std::ostream& p_stream, const state_choose_proc& p_state);
};