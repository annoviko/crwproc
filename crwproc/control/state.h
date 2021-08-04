#pragma once

#include <variant>

#include "state_choose_proc.h"
#include "state_create_filter.h"
#include "state_search.h"
#include "state_exit.h"


using state = std::variant<
    state_choose_proc,
    state_create_filter,
    state_search,
    state_exit
>;
