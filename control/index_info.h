/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once


#include <cstddef>
#include <regex>
#include <string>
#include <unordered_set>
#include <unordered_map>


class index_info {
public:
    using user_instruction = std::unordered_map<std::string, std::uint64_t>;

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
    std::string m_instruction;

    user_instruction m_user_instruction;

public:
    index_info(const std::string& p_instruction, const std::size_t p_limit, const bool p_index_only);

    index_info(const std::string& p_instruction, const std::size_t p_limit, const bool p_index_only, const user_instruction& p_index_map);

public:
    std::size_t get_begin() const;

    std::size_t get_end() const;

    bool is_valid() const;

    const std::string& reason() const;

private:
    bool is_all();

    bool is_1st_half();

    bool is_2nd_half();

    void initialize(const std::string& p_instruction);

    bool try_initialize(const std::string& p_instruction);

    bool try_set_all();

    bool try_set_1st_half();

    bool try_set_2nd_half();

    bool try_set_range();

    bool try_set_value();

    bool try_set_user_value();

    void validate();

    void set_invalid_reason();
};