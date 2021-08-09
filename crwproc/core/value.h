#pragma once

#include <cstdint>
#include <iostream>
#include <string>
#include <unordered_map>


class value {
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
    std::uint64_t   m_size = 0;
    type            m_type = value::type::invalid;
    std::string     m_value;

public:
    value() = default;

    value(const type p_type, const std::size_t p_size, const std::string& p_value);

public:
    bool is_valid() const;

    std::uint64_t get_size() const;

    type get_type() const;

    const std::string& get_value() const;

    void set_value(const std::string& p_value);

public:
    friend std::ostream& operator<<(std::ostream& p_stream, const value& p_info);

public:
    static type string_to_type(const std::string& p_type);

    static std::string type_to_string(const type p_type);
};