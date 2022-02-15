/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once

#include <string>
#include <unordered_map>


namespace crwproc {

namespace console {


struct position {
public:
    int x = 0;
    int y = 0;
};


enum class color {
    basic,
    red,
    yellow,
    blue,
    green,
    white,
    black
};


class control {
private:
    static const int BASIC_TEXT_ATTRIBUTE_CODE;
    static const std::unordered_map<color, int> COLOR_FOREGROUND_DICT;
    static const std::unordered_map<color, int> COLOR_BACKGROUND_DICT;
    static const std::unordered_map<color, int> COLOR_RGB_DICT;

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

    static void set_color(const color p_foreground, const color p_background, const bool p_intense = false);

    static void set_foreground_color(const color p_color, const bool p_intense = false);

    static void set_background_color(const color p_color);

    static void set_defaut_color();

    static void set_foreground_default_color();

    static void set_background_default_color();

private:
    static void color_output(const std::string& p_message, const std::uint64_t p_attribute, const bool ask_wait_key);

    static int get_basic_text_attributes();
};


}

}
