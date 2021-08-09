#include "console.h"

#include <windows.h>

#include <conio.h>
#include <iostream>


#define FOREGROUND_YELLOW       14


void console::clear() {
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


void console::error(const std::string& p_message, const bool ask_wait_key) {
    color_output(p_message, FOREGROUND_RED, ask_wait_key);
}


void console::warning(const std::string& p_message, const bool ask_wait_key) {
    color_output(p_message, FOREGROUND_YELLOW, ask_wait_key);
}


void console::color_output(const std::string& p_message, const std::uint64_t p_attribute, const bool ask_wait_key) {
    auto output_function = [ask_wait_key](const std::string& p_message) {
        std::cout << p_message << std::endl;

        if (ask_wait_key) {
            std::cout << "Press any key to continue...";
            _getch();
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
