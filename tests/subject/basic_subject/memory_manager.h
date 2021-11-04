/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once

#include <exception>
#include <string>
#include <vector>

#include "change_type.h"
#include "memory_type.h"
#include "storage.h"
#include "value_type.h"


class memory_manager {
private:
    storage* m_heap_storage = new storage();
    storage  m_stack_storage;

public:
    bool change_value(const value_type p_value_type, const memory_type p_memory_type, const change_type p_change_type, const std::string& p_value);

    std::string get_value(const value_type p_value_type, const memory_type p_memory_type);

    std::uint64_t get_address_value(const value_type p_value_type, const memory_type p_memory_type);

    std::uint64_t get_address_sequence();

    std::vector<std::uint8_t>& get_sequence();

private:
    storage& get_storage(const memory_type p_type);


    template <typename TypeValue>
    TypeValue s2v(const std::string& p_value) {
        if constexpr (std::is_floating_point<TypeValue>::value) {
            return static_cast<TypeValue>(std::stod(p_value));
        }
        else if (std::is_unsigned<TypeValue>::value) {
            return static_cast<TypeValue>(std::stoull(p_value));
        }
        else {
            return static_cast<TypeValue>(std::stoll(p_value));
        }
    }


    template <typename TypeValue>
    TypeValue change(const change_type p_change_type, const TypeValue p_value, const TypeValue p_other) {
        switch (p_change_type) {
        case change_type::add:
            return p_value + p_other;

        case change_type::sub:
            return p_value - p_other;

        case change_type::set:
            return p_other;

        default:
            throw std::exception("Unknown change type.");
        }
    }
};
