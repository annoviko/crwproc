/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "console_table_style.h"


void console_table_style::set_cell_padding_left(const std::size_t p_padding) {
    m_cell_padding_left = p_padding;
}


std::size_t console_table_style::get_cell_padding_left() const {
    return m_cell_padding_left;
}


void console_table_style::set_cell_padding_right(const std::size_t p_padding) {
    m_cell_padding_right = p_padding;
}


std::size_t console_table_style::get_cell_padding_right() const {
    return m_cell_padding_right;
}


void console_table_style::set_table_padding_left(const std::size_t p_padding) {
    m_table_padding_left = p_padding;
}


std::size_t console_table_style::get_table_padding_left() const {
    return m_table_padding_left;
}


void console_table_style::set_header_font_color(const color p_color) {
    m_header_font_color = p_color;
}


color console_table_style::get_header_font_color() const {
    return m_header_font_color;
}


void console_table_style::set_content_font_color(const color p_color) {
    m_content_font_color = p_color;
}


color console_table_style::get_content_font_color() const {
    return m_content_font_color;
}
