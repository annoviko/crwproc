/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "memory_block.h"


memory_block::memory_block(const std::uint64_t p_begin, const std::uint64_t p_end) :
    m_begin(p_begin),
    m_end(p_end)
{ }


bool memory_block::is_empty() const {
    return m_values.empty() && (m_memory == nullptr);
}


bool memory_block::is_raw_memory() const {
    return m_memory != nullptr;
}


std::uint64_t memory_block::get_begin() const {
    return m_begin;
}


std::uint64_t memory_block::get_end() const {
    return m_end;
}


std::uint64_t memory_block::get_region_size() const {
    return m_end - m_begin;
}


std::uint64_t memory_block::get_amount(const std::size_t p_value_size) const {
    if (m_memory != nullptr) {
        return get_region_size() - p_value_size + 1;
    }

    return m_values.size();
}


const proc_pointer_collection& memory_block::get_values() const {
    return m_values;
}


proc_pointer_collection& memory_block::get_values() {
    return m_values;
}


void memory_block::add_value(const proc_pointer& p_pointer) {
    const std::uint64_t address_value = p_pointer.get_address();

    m_begin = std::min(m_begin, address_value);
    m_end = std::max(m_end, address_value);

    m_values.push_back(p_pointer);
}


const memory_block::raw_memory& memory_block::get_memory() const {
    return m_memory;
}


void memory_block::set_memory(const raw_memory& p_memory) {
    m_memory = p_memory;
}


void memory_block::clean() {
    clean_values();
    clean_memory();
}


void memory_block::clean_values() {
    m_values.clear();
}


void memory_block::clean_memory() {
    m_memory.reset();
}
