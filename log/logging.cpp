/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "logging.h"


std::string logging::PREFIX = "crwproc";


void logging::initialize(const std::string& p_prefix) {
    PREFIX = p_prefix;
}


log_writer& logging::instance() {
    static log_writer writer(PREFIX);
    return writer;
}
