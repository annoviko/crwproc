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
        static constexpr char exit[]            = "\\exit";
        static constexpr char choose[]          = "\\choose";
        static constexpr char create_filter[]   = "\\create_filter";
        static constexpr char update_filter[]   = "\\update_filter";
        static constexpr char add[]             = "\\add";
        static constexpr char show[]            = "\\show";
        static constexpr char edit[]            = "\\edit";
        static constexpr char set[]             = "\\set";
        static constexpr char refresh[]         = "\\refresh";
        static constexpr char remove[]          = "\\remove";
        static constexpr char help[]            = "\\help";
    };

private:
    static const std::unordered_map<std::string, event> COMMANDS;

public:
    static const std::unordered_map<std::string, std::string> DESCRIPTION;

    static const std::unordered_map<std::string, std::vector<std::string>> EXAMPLES;

public:
    static event to_event(const std::string& p_command);
};


using command_collection = std::set<std::string>;