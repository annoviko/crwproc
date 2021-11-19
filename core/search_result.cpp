/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "search_result.h"

#include <iterator>
#include <numeric>


search_result::search_result(const type_desc& p_type) :
    m_info(p_type)
{ }


search_result::iterator search_result::begin() {
    proc_pointer_collection::iterator iter_pointer;
    if (!m_blocks.empty()) {
        iter_pointer = m_blocks.front().get_values().begin();
    }

    return iterator{ m_blocks.begin(), m_blocks.end(), iter_pointer };
}


search_result::const_iterator search_result::begin() const {
    proc_pointer_collection::const_iterator iter_pointer;
    if (!m_blocks.empty()) {
        iter_pointer = m_blocks.front().get_values().cbegin();
    }

    return const_iterator{ m_blocks.cbegin(), m_blocks.cend(), iter_pointer };
}


search_result::iterator search_result::end() {
    proc_pointer_collection::iterator iter_pointer;
    if (!m_blocks.empty()) {
        iter_pointer = m_blocks.back().get_values().end();
    }

    return iterator{ m_blocks.end(), m_blocks.end(), iter_pointer };
}


search_result::const_iterator search_result::end() const {
    proc_pointer_collection::const_iterator iter_pointer;
    if (!m_blocks.empty()) {
        iter_pointer = m_blocks.back().get_values().end();
    }

    return const_iterator{ m_blocks.cend(), m_blocks.cend(), iter_pointer };
}


search_result::iterator search_result::erase(const search_result::iterator& p_iter) {
    iterator result = p_iter;
    result.m_iter_pointer = p_iter.m_iter_block->get_values().erase(p_iter.m_iter_pointer);
    if ((result.m_iter_pointer == result.m_iter_block->get_values().end()) && (result.m_iter_block != result.m_iter_end_block)) {
        ++result.m_iter_block;
        result.m_iter_pointer = result.m_iter_block->get_values().begin();
    }

    return result;
}


search_result::iterator search_result::erase(const search_result::iterator& p_begin, const search_result::iterator& p_end) {
    iterator result = p_begin;

    while(result.m_iter_block != p_end.m_iter_block) {
        result.m_iter_pointer = result.m_iter_block->get_values().erase(result.m_iter_pointer, result.m_iter_block->get_values().end());

        ++result.m_iter_block;
        if (result.m_iter_pointer != p_begin.m_iter_pointer) {
            result.m_iter_pointer = result.m_iter_block->get_values().begin();
        }
    }

    result.m_iter_pointer = result.m_iter_block->get_values().erase(result.m_iter_pointer, p_end.m_iter_pointer);
    return result;
}


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
    const std::size_t amount = std::accumulate(m_blocks.begin(), m_blocks.end(), static_cast<std::size_t>(0), [this](std::size_t p_value, const memory_block& p_block) {
        return p_value + p_block.get_amount(m_info.get_size());
    });

    return amount;
}


std::uint64_t search_result::get_size() const {
    return get_amount_values() * m_info.get_size();
}
