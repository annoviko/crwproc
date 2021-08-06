#pragma once

#include <string>

#include "control/context.h"


class intro_builder {
public:
    static void show(const context& p_context, const std::string& p_intro);
};