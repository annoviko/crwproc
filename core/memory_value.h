/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/

#pragma once


#include <cstdint>
#include <memory>
#include <string>

#include "type_desc.h"


class memory_value {
private:
    std::uint8_t    m_buffer[sizeof(std::uint64_t)];

public:
    memory_value() = default;

    memory_value(const void* p_buffer, const std::size_t p_size);

public:
    const void* get() const;

    void set(const void* p_buffer, const std::size_t p_size);

    void set(const std::string& p_value, const type_desc& p_type);

    std::string to_string(const type_desc& p_type) const;

public:
    template <typename TypeValue>
    static memory_value create(const TypeValue p_value) {
        return memory_value((void*)&p_value, sizeof(TypeValue));
    }

    template <typename TypeValue>
    TypeValue get() const {
        return *((TypeValue*)m_buffer);
    }

    template <typename TypeValue>
    void set(const TypeValue p_value) {
        *((TypeValue*)m_buffer) = p_value;
    }
};