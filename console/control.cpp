/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "control.h"

#include <windows.h>

#include <iostream>


#undef max


namespace crwproc {

namespace console {


constexpr int FOREGROUND_YELLOW = FOREGROUND_RED | FOREGROUND_GREEN;
constexpr int BACKGROUND_YELLOW = FOREGROUND_YELLOW << 4;

constexpr int FOREGROUND_WHITE = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
constexpr int BACKGROUND_WHITE = FOREGROUND_WHITE << 4;

constexpr int FOREGROUND_BLACK = 0;
constexpr int BACKGROUND_BLACK = 0;


const int control::BASIC_TEXT_ATTRIBUTE_CODE = control::get_basic_text_attributes();


const std::unordered_map<color, int> control::COLOR_FOREGROUND_DICT = {
    { color::basic, control::BASIC_TEXT_ATTRIBUTE_CODE & 0xFF0F },
    { color::red, FOREGROUND_RED },
    { color::blue, FOREGROUND_BLUE },
    { color::green, FOREGROUND_GREEN },
    { color::yellow, FOREGROUND_YELLOW },
    { color::white, FOREGROUND_WHITE },
    { color::black, FOREGROUND_BLACK }
};

const std::unordered_map<color, int> control::COLOR_BACKGROUND_DICT = {
    { color::basic, control::BASIC_TEXT_ATTRIBUTE_CODE & 0xFFF0 },
    { color::red, BACKGROUND_RED },
    { color::blue, BACKGROUND_BLUE },
    { color::green, BACKGROUND_GREEN },
    { color::yellow, BACKGROUND_YELLOW },
    { color::white, BACKGROUND_WHITE },
    { color::black, BACKGROUND_BLACK }
};


void control::clear() {
    HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO buffer_info;
    SMALL_RECT scroll_rectange;
    COORD scroll_target;
    CHAR_INFO fill;

    if (!GetConsoleScreenBufferInfo(console_handle, &buffer_info)) {
        return;
    }

    scroll_rectange.Left = 0;
    scroll_rectange.Top = 0;
    scroll_rectange.Right = buffer_info.dwSize.X;
    scroll_rectange.Bottom = buffer_info.dwSize.Y;

    scroll_target.X = 0;
    scroll_target.Y = (SHORT)(0 - buffer_info.dwSize.Y);

    fill.Char.UnicodeChar = TEXT(' ');
    fill.Attributes = buffer_info.wAttributes;

    ScrollConsoleScreenBuffer(console_handle, &scroll_rectange, nullptr, scroll_target, &fill);

    buffer_info.dwCursorPosition.X = 0;
    buffer_info.dwCursorPosition.Y = 0;

    SetConsoleCursorPosition(console_handle, buffer_info.dwCursorPosition);
}


void control::clear_line() {
    control::clear_lines(1);
}


void control::clear_lines(const std::size_t p_lines) {
    HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO buffer_info;

    std::memset((void*)&buffer_info, 0x00, sizeof(CONSOLE_SCREEN_BUFFER_INFO));

    GetConsoleScreenBufferInfo(console_handle, &buffer_info);

    SMALL_RECT scroll_rectange;
    scroll_rectange.Left = buffer_info.dwCursorPosition.X;
    scroll_rectange.Top = buffer_info.dwCursorPosition.Y;
    scroll_rectange.Right = buffer_info.dwSize.X;
    scroll_rectange.Bottom = static_cast<short>(p_lines);

    COORD scroll_target;
    scroll_target.X = 0;
    scroll_target.Y = (SHORT)(0 - p_lines);

    CHAR_INFO fill;
    fill.Char.UnicodeChar = TEXT(' ');
    fill.Attributes = buffer_info.wAttributes;

    ScrollConsoleScreenBuffer(console_handle, &scroll_rectange, nullptr, scroll_target, &fill);

    SetConsoleCursorPosition(console_handle, buffer_info.dwCursorPosition);
}


void control::error(const std::string& p_message) {
    color_output(p_message, FOREGROUND_RED, false);
}


void control::error_and_wait_key(const std::string& p_message) {
    color_output(p_message, FOREGROUND_RED, true);
}


void control::warning(const std::string& p_message) {
    color_output(p_message, FOREGROUND_YELLOW, false);
}


void control::warning_and_wait_key(const std::string& p_message) {
    color_output(p_message, FOREGROUND_YELLOW, true);
}


void control::color_output(const std::string& p_message, const std::uint64_t p_attribute, const bool ask_wait_key) {
    auto output_function = [ask_wait_key](const std::string& p_message) {
        std::cout << p_message << std::endl;

        if (ask_wait_key) {
            std::cout << "Press any key to continue...";

             std::cin.putback('\n');
             std::cin.clear();
             std::cin.ignore(2048, '\n');

             std::cin.get();

#if 0
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            const int value = std::cin.get();
#endif
            std::cout << std::endl;
        }
    };

    HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO buffer_info;
    if (!GetConsoleScreenBufferInfo(console_handle, &buffer_info)) {
        output_function(p_message);
        return;
    }

    SetConsoleTextAttribute(console_handle, static_cast<WORD>(p_attribute) | FOREGROUND_INTENSITY);
    output_function(p_message);
    SetConsoleTextAttribute(console_handle, buffer_info.wAttributes);
}


position control::get_cursor_position() {
    HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO buffer_info;

    std::memset((void*)&buffer_info, 0x00, sizeof(CONSOLE_SCREEN_BUFFER_INFO));

    GetConsoleScreenBufferInfo(console_handle, &buffer_info);
    return { buffer_info.dwCursorPosition.X, buffer_info.dwCursorPosition.Y };
}


void control::set_cursor_position(const position& p_position) {
    HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleCursorPosition(console_handle, { static_cast<short>(p_position.x), static_cast<short>(p_position.y) });
}


void control::set_color(const color p_foreground, const color p_background, const bool p_intense) {
    set_foreground_color(p_foreground, p_intense);
    set_background_color(p_background);
}


void control::set_foreground_color(const color p_color, const bool p_intense) {
    const auto iter = COLOR_FOREGROUND_DICT.find(p_color);
    if (iter != COLOR_FOREGROUND_DICT.cend()) {
        const HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO buffer_info;

        GetConsoleScreenBufferInfo(console_handle, &buffer_info);

        const WORD intensity = p_intense ? FOREGROUND_INTENSITY : 0;
        const WORD attributes = (buffer_info.wAttributes & 0xFFF0) | static_cast<WORD>(iter->second) | intensity;

        SetConsoleTextAttribute(console_handle, attributes);
    }
}


void control::set_background_color(const color p_color) {
    const auto iter = COLOR_BACKGROUND_DICT.find(p_color);
    if (iter != COLOR_BACKGROUND_DICT.cend()) {
        const HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO buffer_info;

        GetConsoleScreenBufferInfo(console_handle, &buffer_info);
        const WORD attributes = (buffer_info.wAttributes & 0xFF0F) | static_cast<WORD>(iter->second);

        SetConsoleTextAttribute(console_handle, attributes);
    }
}


void control::set_defaut_color() {
    set_foreground_color(color::basic);
    set_background_color(color::basic);
}


void control::set_foreground_default_color() {
    set_foreground_color(color::basic);
}


void control::set_background_default_color() {
    set_background_color(color::basic);
}


int control::get_basic_text_attributes() {
    HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO buffer_info;

    GetConsoleScreenBufferInfo(console_handle, &buffer_info);
    return buffer_info.wAttributes;
}


}

}
