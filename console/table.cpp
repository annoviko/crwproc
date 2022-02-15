/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "table.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <stdexcept>


#undef max


namespace crwproc {

namespace console {


table::table(const std::size_t p_rows, const std::size_t p_cols, const table_style& p_style) :
    m_table(p_rows, table_row(p_cols)),
    m_col_props(p_cols),
    m_col_width(p_cols, 0),
    m_style(p_style)
{ }


std::size_t table::get_amount_cols() const {
    return m_table.empty() ? 0 : m_table[0].size();
}


std::size_t table::get_amount_rows() const {
    return m_table.size();
}


void table::set_column_name(const std::size_t p_index, const std::string& p_name) {
    if (p_index >= get_amount_cols()) {
        throw std::out_of_range("column index '" + std::to_string(p_index) + "' is out of range due to table size '"
            + std::to_string(get_amount_rows()) + "x" + std::to_string(get_amount_cols()) + "'");
    }

    m_table[0][p_index] = p_name;
    m_col_width[p_index] = std::max(p_name.size(), m_col_width[p_index]);
}


void table::set_column_names(const table::table_row& p_names) {
    for (std::size_t i = 0; i < p_names.size(); i++) {
        set_column_name(i, p_names[i]);
    }
}


void table::set_row_content(const std::size_t p_row_index, const table_row& p_names) {
    for (std::size_t col_index = 0; col_index < p_names.size(); col_index++) {
        set_cell_content(p_row_index, col_index, p_names[col_index]);
    }
}


void table::set_cell_content(const std::size_t p_row_index, const std::size_t p_col_index, const std::string& p_content) {
    if (p_row_index >= get_amount_rows()) {
        throw std::out_of_range("row index '" + std::to_string(p_row_index) + "' is out of range due to table size '"
            + std::to_string(get_amount_rows()) + "x" + std::to_string(get_amount_cols()) + "'");
    }

    if (p_col_index >= get_amount_cols()) {
        throw std::out_of_range("column index '" + std::to_string(p_col_index) + "' is out of range due to table size '"
            + std::to_string(get_amount_rows()) + "x" + std::to_string(get_amount_cols()) + "'");
    }

    m_table[p_row_index][p_col_index] = p_content;
    m_col_width[p_col_index] = std::max(p_content.size(), m_col_width[p_col_index]);
}


table_cell_property& table::get_col_property(const std::size_t p_index) {
    if (p_index >= get_amount_cols()) {
        throw std::out_of_range("column index '" + std::to_string(p_index) + "' is out of range due to table size '"
            + std::to_string(get_amount_rows()) + "x" + std::to_string(get_amount_cols()) + "'");
    }

    return m_col_props[p_index];
}


void table::show() const {
    show_header();
    show_content();
}


std::size_t table::get_table_width() const {
    std::size_t width = get_amount_cols() + 1;
    for (const auto p_cell_width : m_col_width) {
        width += p_cell_width;
    }

    return width;
}


void table::show_row_separator() const {
    std::cout << std::string(m_style.get_table_padding_left(), ' ') << '*';
    for (const auto p_cell_width : m_col_width) {
        std::cout << std::string(p_cell_width + m_style.get_cell_padding_left() + m_style.get_cell_padding_right(), '-') << '*';
    }

    std::cout << std::endl;
}


void table::show_header() const {
    show_row_separator();

    std::cout << std::string(m_style.get_table_padding_left(), ' ');
    for (std::size_t i = 0; i < get_amount_cols(); i++) {
        std::cout << '|' << std::string(m_style.get_cell_padding_left(), ' ');

        control::set_foreground_color(m_style.get_header_font_color(), true);
        set_stdout_alignment(m_col_props[i].alignment);
        std::cout << std::setw(m_col_width[i]) << m_table[0][i];
        control::set_foreground_default_color();

        std::cout << std::string(m_style.get_cell_padding_right(), ' ');
    }

    std::cout << '|' << std::endl;
    show_row_separator();
}


void table::show_content_row(const std::size_t p_index) const {
    std::cout << std::string(m_style.get_table_padding_left(), ' ');
    for (std::size_t i = 0; i < get_amount_cols(); i++) {
        std::cout << '|' << std::string(m_style.get_cell_padding_left(), ' ');

        control::set_foreground_color(m_style.get_content_font_color(), true);
        set_stdout_alignment(m_col_props[i].alignment);
        std::cout << std::setw(m_col_width[i]) << m_table[p_index][i];
        control::set_foreground_default_color();

        std::cout << std::string(m_style.get_cell_padding_right(), ' ');
    }
    std::cout << '|' << std::endl;
}


void table::show_content() const {
    for (std::size_t i = 1; i < m_table.size(); i++) {
        show_content_row(i);
    }

    show_row_separator();
}


void table::set_stdout_alignment(const table_cell_alignment& p_alignment) const {
    switch (p_alignment) {
    case table_cell_alignment::left:
        std::cout << std::left;
        break;

    case table_cell_alignment::right:
        std::cout << std::right;
        break;

    default:
        throw std::invalid_argument("unknown alignment '" + std::to_string(static_cast<std::size_t>(p_alignment)) + "' is specified");
    }
}


}

}
