#pragma once


#include <string>


class proc_info {
public:
    static const std::size_t INVALID_PID;

private:
    std::size_t m_pid = INVALID_PID;
    std::string m_name;

public:
    proc_info() = default;

    proc_info(const std::size_t p_pid, const std::string& p_name);

public:
    const std::string & name() const;

    const std::size_t pid() const;

    const bool is_valid() const;
};