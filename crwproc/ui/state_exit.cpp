#include "state_exit.h"


event state_exit::operator()(context& p_context) {
    (void) p_context;
    std::exit(0);
}
