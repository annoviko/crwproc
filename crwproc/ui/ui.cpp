#include "ui.h"


void ui::run() {
    while (true) {
        m_stm.handle_state();
    }
}