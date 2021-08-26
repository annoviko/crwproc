#pragma once

#include <iostream>
#include <string>


class proc_info {
public:
    static const std::size_t INVALID_PID;

private:
    std::size_t     m_pid = INVALID_PID;
    std::string     m_name;
    std::uint64_t   m_address = 0;

public:
    proc_info() = default;

    proc_info(const std::size_t p_pid, const std::string& p_name, const std::uint64_t p_address);

public:
    const std::string & name() const;

    const std::size_t pid() const;

    const std::uint64_t base_address() const;

    const bool is_valid() const;

public:
    friend std::ostream& operator<<(std::ostream& p_stream, const proc_info& p_info);
};