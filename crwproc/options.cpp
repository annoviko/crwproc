#include "options.h"

#include <algorithm>
#include <iterator>


options::options(std::size_t p_argc, char* p_argv[]) {
    handle_options(p_argc, p_argv);
}


const std::optional<std::string>& options::get_log_prefix() const {
    return m_log_prefix;
}


void options::handle_options(std::size_t p_argc, char* p_argv[]) {
    for (std::size_t i = 1; i < p_argc; i++) {
        const std::string option = p_argv[i];
        if ((option == OPTION_LOG_PREFIX) || (option == OPTION_LOG_PREFIX_SHORT)) {
            i = handle_option_log_prefix(i, p_argc, p_argv);
        }
    }
}


std::size_t options::handle_option_log_prefix(std::size_t p_index, std::size_t p_argc, char* p_argv[]) {
    if (p_index + 1 >= p_argc) {
        return INVALID_INDEX;
    }

    std::string option_value_canonical;
    std::string option_value = p_argv[p_index + 1];

    std::transform(option_value.begin(), option_value.end(), std::back_inserter(option_value_canonical), [](const char symbol) {
        return static_cast<char>(std::tolower(symbol));
    });

    m_log_prefix = option_value_canonical;
    return p_index + 1;
}