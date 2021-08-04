#pragma once

#include <unordered_map>

#include "event.h"


class command {
private:
    static const std::unordered_map<std::string, event> COMMANDS;

public:
    static event to_event(const std::string& p_command);
};
