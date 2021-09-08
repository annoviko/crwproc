/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once


#include <cstdint>
#include <memory>
#include <vector>

#include "proc_pointer.h"


using proc_pointer_collection = std::vector<proc_pointer>;


class memory_block {
public:
    using raw_memory = std::shared_ptr<std::uint8_t[]>;

private:
    std::uint64_t m_begin = 0;              /* address of the variable with the smallest address within this block */
    std::uint64_t m_end = 0;                /* address of the variable with highest address within this block */

    proc_pointer_collection m_values;       /* filtered values when for example filter_range or filter_equal are used. */
    raw_memory m_memory = nullptr;          /* memory when initial capture is needed like in case of filter_less or filter_more. */

public:
    memory_block() = default;

    memory_block(const std::uint64_t p_begin, const std::uint64_t p_end);

    memory_block(const memory_block& p_block) = default;

    memory_block(memory_block&& p_block) = default;

public:
    bool is_empty() const;

    bool is_raw_memory() const;

    std::uint64_t get_begin() const;

    std::uint64_t get_end() const;

    std::uint64_t get_region_size() const;

    std::uint64_t get_amount(const std::size_t p_value_size) const;

    const proc_pointer_collection& get_values() const;

    proc_pointer_collection& get_values();

    void add_value(const proc_pointer& p_pointer);

    void shrink_borders_using_values(const std::size_t p_value_size);

    const raw_memory& get_memory() const;

    void set_memory(const raw_memory& p_memory);

    void clean();

    void clean_values();

    void clean_memory();
};