/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "rest_server.h"

#include <iostream>
#include <string>


int main(int argc, char * argv[]) {
    const std::string address = "127.0.0.1";
    const std::size_t port = 51425;

    const std::vector<std::string> options_silent = { "-s", "--silent" };
    bool silent = false;
    for (std::size_t i = 1; i < static_cast<std::size_t>(argc); i++) {
        const std::string user_option = argv[i];
        std::string canonical_user_option;

        std::transform(user_option.begin(), user_option.end(), std::back_inserter(canonical_user_option), [](const char symbol) {
            return static_cast<char>(std::tolower(symbol));
        });

        for (const auto& option : options_silent) {
            if (option == canonical_user_option) {
                silent = true;
                break;
            }
        }
    }

    if (silent) {
        std::cout << "Basic subject is running on '" + address + ":" + std::to_string(port) + "' (pid: '" << GetCurrentProcessId() << "')..." << std::endl;
    }

    rest_server(address, port).run();
    return 0;
}