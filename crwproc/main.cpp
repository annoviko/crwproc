/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include <iostream>

#include "control/ui.h"
#include "log/logging.h"


int main() {
    LOG_INFO("crwproc startup.")

    try {
        ui application;
        application.run();
    }
    catch (std::exception& p_exception) {
        LOG_FATAL("Unexpected critical error: " << p_exception.what())
        std::cout << std::endl << "Unexpected critical error: " << p_exception.what() << std::endl;
    }
    catch (...) {
        LOG_FATAL("Unexpected unknown exception")
        std::cout << std::endl << "Unexpected unknown exception" << std::endl;
    }


    LOG_INFO("crwproc termination.")
    return 0;
}