#include "filter_reader_value.h"

#include <iostream>


std::string filter_reader_value::read(const filter_value& p_filter) {
    std::cout << "Enter value that is going to be searched in the process: ";

    std::string value;
    std::cin >> value;

    try {
        switch (p_filter.get_type()) {
        case filter_value::type::integral:
            std::stoull(value);
            break;

        case filter_value::type::floating:
            std::stof(value);
            break;

        case filter_value::type::doubling:
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
