/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/

#pragma once


#include <cstddef>
#include <iterator>
#include <vector>

#include "memory_block.h"
#include "type_desc.h"


using memory_block_sequence = std::vector<memory_block>;


class search_result {
private:
    template <typename ValueType, typename BlockIterType, typename PointerIterType>
    struct iterator_impl {
    public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = ValueType;
        using pointer           = ValueType*;
        using reference         = ValueType&;

        friend class search_result;

    private:
        BlockIterType m_iter_block;
        BlockIterType m_iter_end_block;
        PointerIterType m_iter_pointer;

    public:
        iterator_impl(const BlockIterType& p_iter_block, 
                      const BlockIterType& p_iter_end_block, 
                      const PointerIterType& p_iter_pointer)
        :
            m_iter_block(p_iter_block),
            m_iter_end_block(p_iter_end_block),
            m_iter_pointer(p_iter_pointer)
        { }

    public:
        reference operator*() const {
            return *m_iter_pointer;
        }


        pointer operator->() const {
            return m_iter_pointer.operator->();
        }


        iterator_impl& operator++() {
            ++m_iter_pointer;

            if (m_iter_pointer == m_iter_block->get_values().end()) {
                ++m_iter_block;
                m_iter_pointer = m_iter_block->get_values().begin();
            }

            return *this;
        }


        iterator_impl operator++(int) {
            const auto result = *this;
            ++(*this);
            return result;
        }


        difference_type operator-(const iterator_impl &p_iterator) const {
            difference_type difference = 0;
            auto cursor = *this;

            while(cursor != p_iterator) {
                ++cursor;
                ++difference;
            }

            return difference;
        }


        friend bool operator== (const iterator_impl& p_left, const iterator_impl& p_right) {
            return (p_left.m_iter_pointer == p_right.m_iter_pointer) && (p_left.m_iter_block == p_right.m_iter_block);
        }


        friend bool operator!= (const iterator_impl& p_left, const iterator_impl& p_right) {
            return !(p_left == p_right);
        }
    };

public:
    using iterator = iterator_impl<proc_pointer, memory_block_sequence::iterator, proc_pointer_collection::iterator>;
    using const_iterator = iterator_impl<const proc_pointer, memory_block_sequence::const_iterator, proc_pointer_collection::const_iterator>;

private:
    type_desc m_info;
    memory_block_sequence m_blocks;

public:
    search_result() = default;

    explicit search_result(const type_desc& p_type);

public:
    iterator begin();

    const_iterator begin() const;

    iterator end();

    const_iterator end() const;

    iterator erase(const iterator& p_iter);

    iterator erase(const iterator& p_begin, const iterator& p_end);

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

    std::uint64_t get_size() const;
};
