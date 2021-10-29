/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once


#include <limits>
#include <optional>
#include <set>
#include <string>


class options {
private:
    static constexpr std::size_t INVALID_INDEX = std::numeric_limits<std::size_t>::max();

    static constexpr char OPTION_LOG_PREFIX[] = "--log-prefix";
    static constexpr char OPTION_LOG_PREFIX_SHORT[] = "-l";

private:
    std::optional<std::string>  m_log_prefix;
    std::set<std::string> m_unavailable;

public:
    options(std::size_t p_argc, char* p_argv[]);

public:
    const std::optional<std::string>& get_log_prefix() const;

private:
    void handle_options(std::size_t p_argc, char* p_argv[]);

    std::size_t handle_option_log_prefix(std::size_t p_index, std::size_t p_argc, char* p_argv[]);
};