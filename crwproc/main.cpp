/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include <iostream>

#include "control/ui.h"

#include "log/logging.h"

#include "options.h"


int main(int argc, char *argv[]) {
    try {
        options config(argc, argv);
        if (config.get_log_prefix()) {
            logging::initialize(config.get_log_prefix().value());
            LOG_INFO("crwproc log prefix '" << config.get_log_prefix().value() << "'.")
        }

        LOG_INFO("crwproc startup.")

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