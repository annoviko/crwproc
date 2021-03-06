/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once


#include <string>
#include <vector>

#include "table_style.h"
#include "table_cell_property.h"


namespace crwproc {

namespace console {


class table {
private:
    using table_row = std::vector<std::string>;
    using table_view = std::vector<table_row>;

    using col_props = std::vector<table_cell_property>;

    using width_sequence = std::vector<std::size_t>;

private:
    table_view m_table;

    col_props m_col_props;

    width_sequence m_col_width;

    table_style m_style;

public:
    table(const std::size_t p_rows, const std::size_t p_cols, const table_style& p_style = table_style{ });

public:
    std::size_t get_amount_cols() const;

    std::size_t get_amount_rows() const;

    void set_column_name(const std::size_t p_index, const std::string& p_name);

    void set_column_names(const table_row& p_names);

    void set_row_content(const std::size_t p_row_index, const table_row& p_names);

    void set_cell_content(const std::size_t p_row_index, const std::size_t p_col_index, const std::string& p_content);

    table_cell_property& get_col_property(const std::size_t p_index);

    void show() const;

private:
    std::size_t get_table_width() const;

    void show_row_separator() const;

    void show_header() const;

    void show_content_row(const std::size_t p_index) const;

    void show_content() const;

    void set_stdout_alignment(const table_cell_alignment& p_property) const;
};


}

}
