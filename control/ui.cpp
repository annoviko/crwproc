/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "ui.h"


void ui::run() {
    while (true) {
        m_stm.handle_state();
    }
}