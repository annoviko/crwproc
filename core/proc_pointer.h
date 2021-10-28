/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once

#include <cstdint>

#include "memory_value.h"


class proc_pointer {
public:
    static constexpr std::uint64_t INVALID_ADDRESS = 0;

private:
    std::uint64_t   m_address = INVALID_ADDRESS;
    memory_value    m_value;

public:
    proc_pointer() = default;

    explicit proc_pointer(const std::uint64_t p_address);

    proc_pointer(const std::uint64_t p_address, const void* p_buffer, const std::size_t p_size);

public:
    bool is_valid() const;

    void invalidate();

    std::uint64_t get_address() const;

    const memory_value& get_value() const;

    memory_value& get_value();

public:
    template <typename TypeValue>
    static proc_pointer create(const std::uint64_t p_address, const TypeValue p_value) {
        return proc_pointer(p_address, (void*)&p_value, sizeof(TypeValue));
    }

public:
    bool operator==(const proc_pointer& p_pointer) const;
};
