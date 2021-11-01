/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once


#include <cstddef>
#include <regex>
#include <string>
#include <unordered_set>


class index_info {
private:
    static const std::unordered_set<std::string> INSTRUCTION_ALL;
    static const std::unordered_set<std::string> INSTRUCTION_1ST_HALF;
    static const std::unordered_set<std::string> INSTRUCTION_2ND_HALF;

    static const std::regex PATTERN_RANGE;

private:
    bool m_index_only = false;

    std::size_t m_limit = 0;
    std::size_t m_begin = 0;
    std::size_t m_end = 0;

    bool m_valid = false;
    std::string m_reason;

public:
    index_info(const std::string& p_instruction, const std::size_t p_limit, const bool p_index_only);

public:
    std::size_t get_begin() const;

    std::size_t get_end() const;

    bool is_valid() const;

    const std::string& reason() const;

private:
    static bool is_all(const std::string& p_instruction);

    static bool is_1st_half(const std::string& p_instruction);

    static bool is_2nd_half(const std::string& p_instruction);

    bool try_initialize(const std::string& p_instruction);

    bool try_set_all(const std::string& p_instruction);

    bool try_set_1st_half(const std::string& p_instruction);

    bool try_set_2nd_half(const std::string& p_instruction);

    bool try_set_range(const std::string& p_instruction);

    bool try_set_value(const std::string& p_instruction);

    void validate();

    void set_invalid_reason();
};