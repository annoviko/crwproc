#pragma once

#include "context.h"
#include "event.h"

#include "core/proc_table.h"


class state_choose_proc {
public:
    event operator()(context& p_context);

private:
    void show_procs(const proc_collection& p_table) const;
    std::string ask_proc() const;

    event process_user_input(const std::string& p_input, const proc_collection& p_table, context& p_context) const;
    std::size_t try_get_pid(const std::string& p_input) const;
    event assign_pid(const std::size_t p_pid, const proc_collection& p_table, context& p_context) const;
    event process_user_command(const std::string& p_command) const;
};