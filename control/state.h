#pragma once

#include <variant>

#include "state_choose_proc.h"
#include "state_create_filter.h"
#include "state_search.h"
#include "state_show_search_result.h"
#include "state_update_filter_value.h"
#include "state_edit.h"
#include "state_help.h"
#include "state_exit.h"


using state = std::variant<
    state_choose_proc,
    state_create_filter,
    state_search,
    state_show_search_result,
    state_update_filter_value,
    state_edit,
    state_help,
    state_exit
>;