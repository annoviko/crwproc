/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "index_info.h"

#include <algorithm>
#include <iterator>


const std::unordered_set<std::string> index_info::INSTRUCTION_ALL = { "all" };

const std::unordered_set<std::string> index_info::INSTRUCTION_1ST_HALF = { "first_half", "1half" };

const std::unordered_set<std::string> index_info::INSTRUCTION_2ND_HALF = { "second_half", "2half" };

const std::regex index_info::PATTERN_RANGE("(\\d+)-(\\d+)");


index_info::index_info(const std::string& p_instruction, const std::size_t p_limit, const bool p_index_only) :
    m_index_only(p_index_only),
    m_limit(p_limit)
{
    if (try_initialize(p_instruction)) {
        validate();
    }
    else {
        m_reason = "impossible to parse input argument '" + p_instruction + "'";
    }
}


std::size_t index_info::get_begin() const {
    return m_begin;
}


std::size_t index_info::get_end() const {
    return m_end;
}


bool index_info::is_valid() const {
    return m_valid;
}


const std::string& index_info::reason() const {
    return m_reason;
}


bool index_info::is_all(const std::string& p_instruction) {
    return INSTRUCTION_ALL.find(p_instruction) != INSTRUCTION_ALL.cend();
}

bool index_info::is_1st_half(const std::string& p_instruction) {
    return INSTRUCTION_1ST_HALF.find(p_instruction) != INSTRUCTION_1ST_HALF.cend();
}


bool index_info::is_2nd_half(const std::string& p_instruction) {
    return INSTRUCTION_2ND_HALF.find(p_instruction) != INSTRUCTION_2ND_HALF.cend();
}


bool index_info::try_initialize(const std::string& p_instruction) {
    std::string canonical_instruction;
    std::transform(p_instruction.begin(), p_instruction.end(), std::back_inserter(canonical_instruction), [](const char symbol) {
        return static_cast<char>(std::tolower(symbol));
    });

    if (m_index_only) {
        return try_set_value(canonical_instruction);
    }

    return try_set_all(canonical_instruction) ||
        try_set_1st_half(canonical_instruction) ||
        try_set_2nd_half(canonical_instruction) ||
        try_set_range(canonical_instruction) ||
        try_set_value(canonical_instruction);
}


bool index_info::try_set_all(const std::string& p_instruction) {
    if (is_all(p_instruction)) {
        m_begin = 0;
        m_end = m_limit;

        return true;
    }

    return false;
}

bool index_info::try_set_1st_half(const std::string& p_instruction) {
    if (is_1st_half(p_instruction)) {
        m_begin = 0;
        m_end = static_cast<std::size_t>(std::ceil(static_cast<double>(m_limit) / 2.0));

        return true;
    }

    return false;
}


bool index_info::try_set_2nd_half(const std::string& p_instruction) {
    if (is_2nd_half(p_instruction)) {
        m_begin = static_cast<std::size_t>(std::ceil(static_cast<double>(m_limit) / 2.0));
        m_end = m_limit;

        return true;
    }

    return false;
}


bool index_info::try_set_range(const std::string& p_instruction) {
    std::smatch groups;
    if (std::regex_match(p_instruction, groups, PATTERN_RANGE)) {
        m_begin = std::stoull(groups[1]);
        m_end = std::stoull(groups[2]);

        return true;
    }

    return false;
}


bool index_info::try_set_value(const std::string& p_instruction) {
    try {
        m_begin = std::stoul(p_instruction);
        m_end = m_begin + 1;
    }
    catch(std::exception&) {
        return false;
    }

    return true;
}


void index_info::validate() {
    m_valid = (m_begin < m_end) && (m_end <= m_limit);

    if (!m_valid && m_reason.empty()) {
        set_invalid_reason();
    }
}


void index_info::set_invalid_reason() {
    if (m_end > m_limit) {
        m_reason = "limit value '" + std::to_string(m_limit) + "' cannot be exceeded";
    }
    else if (m_begin >= m_end) {
        m_reason = "empty range (from '" + std::to_string(m_begin) + "' to '" + std::to_string(m_end) + "')";
    }
}
