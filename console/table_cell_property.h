/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once


namespace crwproc {

namespace console {


enum class table_cell_alignment {
    left,
    right
};


struct table_cell_property {
    table_cell_alignment alignment = table_cell_alignment::right;
};


}

}
