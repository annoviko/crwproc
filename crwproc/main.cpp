#include <iostream>

#include "control/ui.h"


int main() {
    try {
        ui application;
        application.run();
    }
    catch (std::exception& p_exception) {
        std::cout << "Unexpected critical error: " << p_exception.what() << std::endl;
    }
    catch (...) {
        std::cout << "Unexpected unknown exception" << std::endl;
    }
    return 0;
}