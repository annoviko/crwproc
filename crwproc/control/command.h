#pragma once

#include <set>
#include <string>
#include <unordered_map>

#include "event.h"


class command {
public:
    class name {
    private:
        name() = delete;

    public:
        static constexpr char exit[]    = "\\exit";
        static constexpr char choose[]  = "\\choose";
        static constexpr char filter[]  = "\\filter";
        static constexpr char add[]     = "\\add";
        static constexpr char show[]    = "\\show";
    };

private:
    static const std::unordered_map<std::string, event> COMMANDS;

public:
    static event to_event(const std::string& p_command);
};


using command_collection = std::set<std::string>;
