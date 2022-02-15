/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once


#include <cstddef>

#include "control.h"


namespace crwproc {

namespace console {


class table_style {
private:
    constexpr static std::size_t CELL_PADDING_LEFT = 1;
    constexpr static std::size_t CELL_PADDING_RIGHT = 1;

    constexpr static std::size_t TABLE_PADDING_LEFT = 4;

private:
    std::size_t m_cell_padding_left = CELL_PADDING_LEFT;
    std::size_t m_cell_padding_right = CELL_PADDING_RIGHT;

    std::size_t m_table_padding_left = TABLE_PADDING_LEFT;

    color m_header_font_color = color::blue;
    color m_content_font_color = color::white;

public:
    table_style() = default;

public:
    void set_cell_padding_left(const std::size_t p_padding);

    std::size_t get_cell_padding_left() const;

    void set_cell_padding_right(const std::size_t p_padding);

    std::size_t get_cell_padding_right() const;

    void set_table_padding_left(const std::size_t p_padding);

    std::size_t get_table_padding_left() const;

    void set_header_font_color(const color p_color);

    color get_header_font_color() const;

    void set_content_font_color(const color p_color);

    color get_content_font_color() const;
};


}

}
