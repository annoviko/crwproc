/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once

#include <set>
#include <string>
#include <unordered_map>

#include "event.h"


class command {
public:
    using description_dict = std::unordered_map<std::string, std::string>;
    using example_dict = std::unordered_map<std::string, std::vector<std::string>>;

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
        static constexpr char address[]         = "\\address";
        static constexpr char refresh[]         = "\\refresh";
        static constexpr char remove[]          = "\\remove";
        static constexpr char help[]            = "\\help";
        static constexpr char save[]            = "\\save";
        static constexpr char load[]            = "\\load";
        static constexpr char revert[]          = "\\revert";
        static constexpr char dump[]            = "\\dump";
        static constexpr char find_sequence[]   = "\\find_sequence";
        static constexpr char rename[]          = "\\rename";
    };

private:
    static const std::unordered_map<std::string, event> COMMANDS;

    static const std::unordered_map<std::string, std::string> DESCRIPTION;

    static const std::unordered_map<std::string, std::vector<std::string>> EXAMPLES;

public:
    static event to_event(const std::string& p_command);

    static const description_dict& get_description();

    static const example_dict& get_examples();

    static void throw_if_command(const std::string& p_user_input);
};


using command_collection = std::set<std::string>;
