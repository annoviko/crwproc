/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "state_exit.h"

#include "log/logging.h"


event state_exit::operator()(context& p_context) {
    (void) p_context;

    LOG_INFO("Terminate application.")
    std::exit(0);
}


std::ostream& operator<<(std::ostream& p_stream, const state_exit&) {
    p_stream << "state_exit";
    return p_stream;
}
