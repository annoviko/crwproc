/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once


#include <iostream>


enum class console_table_cell_alignment {
    left,
    right
};


struct console_table_cell_property {
    console_table_cell_alignment alignment = console_table_cell_alignment::right;
};
