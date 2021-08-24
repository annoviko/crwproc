#pragma once

#include <string>


struct position {
public:
    int x = 0;
    int y = 0;
};


class console {
public:
    static void clear();

    static void clear_line();

    static void clear_lines(const std::size_t p_lines);

    static void error(const std::string& p_message);

    static void error_and_wait_key(const std::string& p_message);

    static void warning(const std::string& p_message);

    static void warning_and_wait_key(const std::string& p_message);

    static position get_cursor_position();

    static void set_cursor_position(const position& p_position);

private:
    static void color_output(const std::string& p_message, const std::uint64_t p_attribute, const bool ask_wait_key);
};