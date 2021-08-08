#include "state_edit.h"


event state_edit::operator()(context& p_context) {
    show_table(p_context);

    return event_done{};
}


void state_edit::show_table(context& p_context) {

}