#pragma once

#include <string>


class console {
public:
    static void clear();

    static void error(const std::string& p_message, const bool p_ask_wait_key);

    static void warning(const std::string& p_message, const bool p_ask_wait_key);

private:
    static void color_output(const std::string& p_message, const std::uint64_t p_attribute, const bool ask_wait_key);
};