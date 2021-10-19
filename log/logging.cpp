/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "logging.h"


log_writer& logging::instance() {
    static log_writer writer("crwproc");
    return writer;
}
