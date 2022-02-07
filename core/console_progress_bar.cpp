#include "console_progress_bar.h"

#include <iostream>
#include <sstream>
#include <string>


const std::string console_progress_bar::PROGRESS_TITLE = "Progress:";


console_progress_bar::console_progress_bar(const std::size_t p_width) :
    m_width(p_width)
{ }


void console_progress_bar::display() {
    m_vertical_position = console::get_cursor_position().y;

    initialize_progress_intro();
    update_percent(0);
}


void console_progress_bar::update(const std::size_t p_progress) {
    update_percent(p_progress);
    update_progress(p_progress);
}


void console_progress_bar::update_percent(const std::size_t p_value) const {
    const auto previous_position = console::get_cursor_position();
    console::set_cursor_position({ static_cast<int>(m_percent_position), static_cast<int>(m_vertical_position) });

    console::set_color(color::black, color::green);

    std::string string_value = std::to_string(p_value) + "%";
    const std::size_t filler_length = WIDTH_PROGRESS - string_value.size();
    string_value = std::string(filler_length, ' ') + string_value;

    std::cout << string_value;

    console::set_defaut_color();
    console::set_cursor_position(previous_position);
}


void console_progress_bar::update_progress(const std::size_t p_value) {
    const auto previous_position = console::get_cursor_position();
    console::set_cursor_position({ static_cast<int>(m_begin_progress_position), static_cast<int>(m_vertical_position) });

    const std::size_t amount_bars = (p_value * m_width_progress) / 100;
    std::cout << std::string(static_cast<int>(amount_bars), '#');

    console::set_cursor_position(previous_position);
}


void console_progress_bar::initialize_progress_intro() {
    std::size_t position = show_title();
    position = initialize_percent(position);
    position = initialize_progress(position);
}


std::size_t console_progress_bar::show_title() {
    console::set_color(color::black, color::green);

    std::cout << PROGRESS_TITLE << SPACE;

    console::set_defaut_color();

    return PROGRESS_TITLE.size() + 1;
}


std::size_t console_progress_bar::initialize_percent(const std::size_t& p_position) {
    std::stringstream stream;
    stream << LEFT_BORDER << std::string(WIDTH_PROGRESS, ' ') << RIGHT_BORDER;

    m_percent_position = p_position + 1;

    console::set_color(color::black, color::green);

    std::cout << stream.str();

    console::set_defaut_color();

    return p_position + stream.str().size();
}


std::size_t console_progress_bar::initialize_progress(const std::size_t& p_position) {
    m_width_progress = m_width - p_position - 3; /* minus left, right border and space of the progress */

    std::stringstream stream;
    stream << SPACE << LEFT_BORDER << std::string(m_width_progress, '.') << RIGHT_BORDER;

    m_begin_progress_position = p_position + 2;  /* plus one to ignore left border and space */
    m_end_progress_position = m_begin_progress_position + m_width_progress;

    std::cout << stream.str();

    return p_position + stream.str().size();
}
