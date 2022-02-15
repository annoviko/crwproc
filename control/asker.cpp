/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "asker.h"

#include <iostream>
#include <sstream>
#include <string>

#include "console/control.h"
#include "core/string_numeric.h"

#include "command.h"
#include "log_wrapper.h"



std::string asker::ask_user_input(const std::string& p_message, const bool p_interruptible) {
    if (!p_message.empty()) {
        std::cout << p_message;
    }

    std::string user_input;
    std::cin >> user_input;

    LOG_INFO("User input: '" << user_input << "'.")

    if (p_interruptible) {
        command::throw_if_command(user_input);
    }

    return user_input;
}


index_info asker::ask_index(const std::size_t p_limit, const bool p_index_only, const bool p_interruptible) {
    LOG_INFO("Ask for user input - index.")

    std::string user_input = ask_user_input(std::string(), p_interruptible);

    index_info info(user_input, p_limit, p_index_only);
    if (!info.is_valid()) {
        LOG_ERROR_WITH_WAIT_KEY_AND_RETURN_VALUE("Error: " + info.reason() + ".", info)
    }

    return info;
}


index_info asker::ask_index(const std::size_t p_limit, const bool p_index_only, const index_info::user_instruction& p_user_instruction, const bool p_interruptible) {
    LOG_INFO("Ask for user input - index with user index map.")

    std::string user_input = ask_user_input(std::string(), p_interruptible);

    index_info info(user_input, p_limit, p_index_only, p_user_instruction);
    if (!info.is_valid()) {
        LOG_ERROR_WITH_WAIT_KEY_AND_RETURN_VALUE("Error: " + info.reason() + ".", info)
    }

    return info;
}


value_type asker::ask_value_type() {
    std::cout << "Select value type that is going to be searched in the process:" << std::endl;

    std::vector<decltype(STR_VALUE_TYPE_DICT)::const_iterator> options;
    options.reserve(STR_VALUE_TYPE_DICT.size());

    for (auto iter = STR_VALUE_TYPE_DICT.cbegin(); iter != STR_VALUE_TYPE_DICT.cend(); iter++) {
        crwproc::console::control::set_foreground_color(crwproc::console::color::blue, true);
        std::cout << " " << options.size();
        crwproc::console::control::set_foreground_default_color();

        std::cout << " - " << iter->first << std::endl;
        options.push_back(iter);
    }

    std::cout << "Enter option number (0-" << options.size() - 1 << "): ";
    const index_info info = asker::ask_index(options.size(), true);

    if (info.is_valid()) {
        std::cout << std::endl;
        return options[info.get_begin()]->second;
    }

    return value_type::invalid;
}


std::size_t asker::ask_value_size() {
    std::cout << "Select value size that is going to be searched in the process:" << std::endl;

    std::vector<std::size_t> options = { 1, 2, 4, 8 };
    for (std::size_t i = 0; i < options.size(); i++) {
        crwproc::console::control::set_foreground_color(crwproc::console::color::blue, true);
        std::cout << " " << i;
        crwproc::console::control::set_foreground_default_color();

        std::cout << " - " << options[i] << " byte" << (options[i] == 1 ? "" : "s") << std::endl;
    }

    std::cout << "Enter option number (0-" << options.size() - 1 << "): ";
    const index_info info = asker::ask_index(options.size(), true);

    if (info.is_valid()) {
        std::cout << std::endl;
        return options[info.get_begin()];
    }

    return INVALID_SIZE;
}


std::optional<bool> asker::ask_value_sign() {
    std::cout << "Select integral value type:" << std::endl;

    std::vector<std::string> options = { "unsigned", "signed" };
    for (std::size_t i = 0; i < options.size(); i++) {
        crwproc::console::control::set_foreground_color(crwproc::console::color::blue, true);
        std::cout << " " << i;
        crwproc::console::control::set_foreground_default_color();

        std::cout << " - " << options[i] << std::endl;
    }


    std::cout << "Enter option number (0-" << options.size() - 1 << "): ";
    const index_info info = asker::ask_index(options.size(), true);

    if (info.is_valid()) {
        std::cout << std::endl;
        return (info.get_begin() == 0) ? false : true;
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
    LOG_INFO("Ask for user input - address.")
    const std::string string_address = ask_user_input("Enter address as a hex value: ");

    const auto address = crwproc::string::stonum<std::uint64_t>(string_address, 16);
    if (!address.has_value()) {
        LOG_ERROR_WITH_WAIT_KEY("Error: address as a hex value is expected (incorrect input: '" + string_address + "').")
    }

    return address;
}


std::optional<std::size_t> asker::ask_length() {
    LOG_INFO("Ask for user input - length.")
    const std::string string_length = ask_user_input("Enter length: ");

    const auto length = crwproc::string::stonum<std::size_t>(string_length);
    if (!length.has_value()) {
        LOG_ERROR_WITH_WAIT_KEY("Error: length as a dec value is expected (incorrect input: '" + string_length + "').")
    }

    return length;
}