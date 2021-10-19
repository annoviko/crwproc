#include "memory_manager.h"

#include <exception>


storage global_storage;


storage& memory_manager::get_storage(const memory_type p_type) {
    switch (p_type) {
    case memory_type::global:
        return global_storage;

    case memory_type::heap:
        return *m_heap_storage;

    case memory_type::stack:
        return m_stack_storage;

    default:
        throw std::exception("Unknown storage type.");
    }
}


std::string memory_manager::get(const value_type p_value_type, const memory_type p_memory_type) {
    storage& content = get_storage(p_memory_type);
    switch (p_value_type) {
    case value_type::i8:
        return std::to_string(content.i8);

    case value_type::i16:
        return std::to_string(content.i16);

    case value_type::i32:
        return std::to_string(content.i32);

    case value_type::i64:
        return std::to_string(content.i64);

    case value_type::u8:
        return std::to_string(content.u8);

    case value_type::u16:
        return std::to_string(content.u16);

    case value_type::u32:
        return std::to_string(content.u32);

    case value_type::u64:
        return std::to_string(content.u64);

    case value_type::flt:
        return std::to_string(content.flt);

    case value_type::dbl:
        return std::to_string(content.dbl);
    }

    return std::string();
}


bool memory_manager::change(const value_type p_value_type, const memory_type p_memory_type, const change_type p_change_type, const std::string& p_value) {
    storage& content = get_storage(p_memory_type);
    switch (p_value_type) {
    case value_type::i8:
        content.i8 = change(p_change_type, content.i8, s2v<std::int8_t>(p_value));
        break;

    case value_type::i16:
        content.i16 = change(p_change_type, content.i16, s2v<std::int16_t>(p_value));
        break;

    case value_type::i32:
        content.i32 = change(p_change_type, content.i32, s2v<std::int32_t>(p_value));
        break;

    case value_type::i64:
        content.i64 = change(p_change_type, content.i64, s2v<std::int64_t>(p_value));
        break;

    case value_type::u8:
        content.u8 = change(p_change_type, content.u8, s2v<std::uint8_t>(p_value));
        break;

    case value_type::u16:
        content.u16 = change(p_change_type, content.u16, s2v<std::uint16_t>(p_value));
        break;

    case value_type::u32:
        content.u32 = change(p_change_type, content.u32, s2v<std::uint32_t>(p_value));
        break;

    case value_type::u64:
        content.u64 = change(p_change_type, content.u64, s2v<std::uint64_t>(p_value));
        break;

    case value_type::flt:
        content.flt = change(p_change_type, content.flt, s2v<float>(p_value));
        break;

    case value_type::dbl:
        content.u64 = change(p_change_type, content.u64, s2v<std::uint64_t>(p_value));
        break;

    default:
        return false;
    }

    return true;
}