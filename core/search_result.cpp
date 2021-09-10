/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "search_result.h"

#include <iterator>


search_result::search_result(const type_desc& p_type) :
    m_info(p_type)
{ }


bool search_result::is_empty() const {
    return m_blocks.empty();
}


void search_result::move(search_result&& p_result) {
    std::move(p_result.m_blocks.begin(), p_result.m_blocks.end(), std::back_inserter(m_blocks));
}


void search_result::reserve(const std::size_t p_size) {
    m_blocks.reserve(p_size);
}


void search_result::clear() {
    m_blocks.clear();
}


const type_desc& search_result::get_type() const {
    return m_info;
}


type_desc& search_result::get_type() {
    return m_info;
}


const memory_block_sequence& search_result::get_memory_blocks() const {
    return m_blocks;
}


memory_block_sequence& search_result::get_memory_blocks() {
    return m_blocks;
}


std::size_t search_result::get_amount_memory_blocks() const {
    return m_blocks.size();
}


std::size_t search_result::get_amount_values() const {
    std::size_t amount = 0;
    for (const auto& p_block : m_blocks) {
        amount += p_block.get_amount(m_info.get_size());
    }

    return amount;
}


std::uint64_t search_result::get_size() const {
    return get_amount_values() * m_info.get_size();
}
