#pragma once

#include <cstddef>

#include "console.h"


class console_progress_bar {
private:
    static constexpr char FILL_BAR = '#';
    static constexpr char EMPTY_BAR = '.';
    static constexpr char LEFT_BORDER = '[';
    static constexpr char RIGHT_BORDER = ']';
    static constexpr char SPACE = ' ';

    static const std::string PROGRESS_TITLE;

    static constexpr color COLOR_FONT_PROGRESS = color::black;
    static constexpr color COLOR_BACKGROUND_PROGRESS = color::green;

    static constexpr std::size_t WIDTH_PROGRESS = 4;

private:
    std::size_t m_width = 0;
    std::size_t m_width_progress = 0;

    std::size_t m_vertical_position = 0;
    std::size_t m_percent_position = 0;
    std::size_t m_begin_progress_position = 0;
    std::size_t m_end_progress_position = 0;

public:
    console_progress_bar(const std::size_t p_width);

public:
    void display();

    void update(const std::size_t p_progress);

private:
    void update_percent(const std::size_t p_value) const;

    void update_progress(const std::size_t p_value);

    void initialize_progress_intro();

    std::size_t show_title();

    std::size_t initialize_percent(const std::size_t& p_position);

    std::size_t initialize_progress(const std::size_t& p_position);
};