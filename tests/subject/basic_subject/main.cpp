/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "rest_server.h"

#include <iostream>


int main() {
    const std::string address = "127.0.0.1";
    const std::size_t port = 51425;

    std::cout << "Basic subject is running on '" + address + ":" + std::to_string(port) + "'..." << std::endl;

    rest_server(address, port).run();
    return 0;
}