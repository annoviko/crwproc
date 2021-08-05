#pragma once

#include <iostream>
#include <list>
#include <string>
#include <unordered_map>


class filter_value {
public:
    enum class type {
        invalid,
        integral,
        floating,
        doubling
    };

public:
    static const std::unordered_map<std::string, type> STR_TYPE_DICT;
    static const std::unordered_map<type, std::string> TYPE_STR_DICT;

private:
    type        m_type = type::invalid;
    std::size_t m_size = 0;
    std::string m_value;

public:
    filter_value() = default;

    filter_value(const type p_type, const std::size_t p_size, const std::string& p_value);

public:
    bool is_valid() const;

    type get_type() const;

    std::size_t get_size() const;

    std::string get_value() const;

public:
    static type to_type(const std::string& p_type);

public:
    friend std::ostream& operator<<(std::ostream& p_stream, const filter_value& p_info);
};
