/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/

#pragma once


#include <vector>

#include "memory_block.h"
#include "type_desc.h"


using memory_block_sequence = std::vector<memory_block>;


class search_result {
private:
    type_desc m_info;
    memory_block_sequence m_blocks;

public:
    search_result() = default;

    search_result(const type_desc& p_type);

public:
    bool is_empty() const;

    void move(search_result&& p_result);

    void reserve(const std::size_t p_size);

    void clear();

    const type_desc& get_type() const;

    type_desc& get_type();

    const memory_block_sequence& get_memory_blocks() const;

    memory_block_sequence& get_memory_blocks();

    std::size_t get_amount_memory_blocks() const;

    std::size_t get_amount_values() const;

    std::size_t get_size() const;
};
