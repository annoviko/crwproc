/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "asker.h"

#include <iostream>
#include <sstream>
#include <string>

#include "core/console.h"

#include "command.h"


std::size_t asker::ask_index(const std::size_t p_limit, const action_index& p_action) {
    std::string user_input;
    std::cin >> user_input;

    command::throw_if_command(user_input);

    std::size_t index_value = 0;

    try {
        index_value = std::stoull(user_input);
    }
    catch (std::exception&) {
        console::error_and_wait_key("Error: unsigned integer value is expected.");
        std::cin.clear();
        std::cin.ignore(256, '\n');
        return INVALID_INDEX;
    }

    if (index_value >= p_limit) {
        console::error_and_wait_key("Error: specified index '" + std::to_string(index_value) + "' is out of range (0-" + std::to_string(p_limit) + ").");
        return INVALID_INDEX;
    }

    if (p_action) {
        p_action(index_value);
    }
    
    return index_value;
}


value_type asker::ask_value_type() {
    std::cout << "Select value type that is going to be searched in the process:" << std::endl;

    std::vector<decltype(STR_TYPE_DICT)::const_iterator> options;
    options.reserve(STR_TYPE_DICT.size());

    for (auto iter = STR_TYPE_DICT.cbegin(); iter != STR_TYPE_DICT.cend(); iter++) {
        console::set_foreground_color(color::blue, true);
        std::cout << " " << options.size();
        console::set_defaut_color();

        std::cout << " - " << iter->first << std::endl;
        options.push_back(iter);
    }

    std::cout << "Enter option number (0-" << options.size() - 1 << "): ";
    std::size_t index_option = asker::ask_index(options.size());

    if (index_option != asker::INVALID_INDEX) {
        std::cout << std::endl;
        return options[index_option]->second;
    }

    return value_type::invalid;
}


std::size_t asker::ask_value_size() {
    std::cout << "Select value size that is going to be searched in the process:" << std::endl;

    std::vector<std::size_t> options = { 1, 2, 4, 8 };
    for (std::size_t i = 0; i < options.size(); i++) {
        console::set_foreground_color(color::blue, true);
        std::cout << " " << i;
        console::set_defaut_color();

        std::cout << " - " << options[i] << " byte" << (options[i] == 1 ? "" : "s") << std::endl;
    }

    std::cout << "Enter option number (0-" << options.size() - 1 << "): ";
    std::size_t index_option = asker::ask_index(options.size());

    if (index_option != asker::INVALID_INDEX) {
        std::cout << std::endl;
        return options[index_option];
    }

    return INVALID_SIZE;
}


std::optional<bool> asker::ask_value_sign() {
    std::cout << "Select integral value type:" << std::endl;

    std::vector<std::string> options = { "unsigned", "signed" };
    for (std::size_t i = 0; i < options.size(); i++) {
        console::set_foreground_color(color::blue, true);
        std::cout << " " << i;
        console::set_defaut_color();

        std::cout << " - " << options[i] << std::endl;
    }


    std::cout << "Enter option number (0-" << options.size() - 1 << "): ";
    std::size_t index_option = asker::ask_index(options.size());

    if (index_option != asker::INVALID_INDEX) {
        std::cout << std::endl;
        return (index_option == 0) ? false : true;
    }

    return { };
}


type_desc asker::ask_type_desc() {
    value_type type = asker::ask_value_type();
    while (type == value_type::invalid) {
        type = asker::ask_value_type();
    }

    std::size_t size = 0;
    bool sign = true;
    if (type == value_type::integral) {
        size = asker::ask_value_size();
        while (size == asker::INVALID_SIZE) {
            size = asker::ask_value_size();
        }

        std::optional<bool> is_signed = asker::ask_value_sign();
        while (!is_signed.has_value()) {
            is_signed = asker::ask_value_sign();
        }

        sign = is_signed.value();
    }

    return type_desc(type, size, sign);
}


std::optional<uint64_t> asker::ask_address() {
    std::cout << "Enter address as a hex value: ";

    std::string string_address;
    std::cin >> string_address;

    command::throw_if_command(string_address);

    std::stringstream stream;
    stream << std::hex << string_address;

    std::uint64_t address;
    stream >> address;

    if (stream.fail()) {
        console::error_and_wait_key("Error: address as a hex value is expected (incorrect input: '" + string_address + "').");
        std::cin.clear();
        std::cin.ignore(256, '\n');
        return { };
    }

    return address;
}
