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
    static constexpr std::int64_t DEFAULT_MULTIPLIER = 1;

private:
    std::uint8_t    m_buffer[sizeof(std::uint64_t)];

public:
    memory_value();

    memory_value(const void* p_buffer, const std::size_t p_size);

public:
    const void* get() const;

    void set(const void* p_buffer, const std::size_t p_size);

    bool set(const std::string& p_value, const type_desc& p_type, const std::int64_t p_multiplier = DEFAULT_MULTIPLIER);

    std::string to_string(const type_desc& p_type, const std::int64_t p_multiplier = DEFAULT_MULTIPLIER) const;

public:
    template <typename TypeValue>
    static memory_value create(const TypeValue p_value) {
        return memory_value((void*)&p_value, sizeof(TypeValue));
    }

    template <typename TypeValue>
    typename std::enable_if<std::is_fundamental<TypeValue>::value, TypeValue>::type
    get() const {
        return *((TypeValue*)m_buffer);
    }

    template <typename TypeValue>
    typename std::enable_if<std::is_fundamental<TypeValue>::value, void>::type
    set(const TypeValue p_value) {
        *((TypeValue*)m_buffer) = p_value;
    }
};
