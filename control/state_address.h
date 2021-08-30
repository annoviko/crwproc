/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once


#include "context.h"
#include "event.h"
#include "state_base.h"

#include "core/proc_info.h"
#include "core/proc_pointer.h"


class state_address : public state_base {
private:
    proc_pointer m_pointer;

public:
    event operator()(context& p_context);

private:
    proc_pointer create_view() const;

    void read(const proc_info& p_info);

    void show_value(const context& p_context) const;

    event ask_next_action(context& p_context);
};