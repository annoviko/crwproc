#include "filter_reader_value.h"

#include <iostream>


std::string filter_reader_value::read(const filter_equal& p_filter) {
    std::cout << "Enter value that is going to be searched in the process: ";

    std::string value;
    std::cin >> value;

    try {
        switch (p_filter.get_value().get_type()) {
        case value::type::integral:
            std::stoull(value);
            break;

        case value::type::floating:
            std::stof(value);
            break;

        case value::type::doubling:
            std::stod(value);
            break;
        }
    }
    catch (...) {
        std::cout << "Error: invalid value is specified '" << value << "'." << std::endl << std::endl;
        return std::string{};
    }

    std::cout << std::endl;
    return value;
}
