#pragma once

#include <variant>

#include "state_choose_proc.h"
#include "state_exit.h"

using state = std::variant<state_choose_proc, state_exit>;
