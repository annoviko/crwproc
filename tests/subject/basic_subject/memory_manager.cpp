/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "memory_manager.h"

#include <exception>
#include <vector>


storage global_storage;

std::vector<std::uint8_t> global_sequence = { 0x10, 0x21, 0x12, 0x23, 0x14, 0x25, 0x16, 0x27, 0x18, 0x29, 0x1A, 0x2B, 0x1C, 0x2D, 0x1E, 0x2F,
                                              0xA0, 0xB1, 0xA2, 0xB3, 0xA4, 0xB5, 0xA6, 0xB7, 0xA8, 0xB9, 0xAA, 0xBB, 0xAC, 0xBD, 0xAE, 0xBF };


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


std::uint64_t memory_manager::get_address_value(const value_type p_value_type, const memory_type p_memory_type) {
    storage& content = get_storage(p_memory_type);
    switch (p_value_type) {
    case value_type::i8:
        return (std::int64_t) &(content.i8);

    case value_type::i16:
        return (std::int64_t) &(content.i16);

    case value_type::i32:
        return (std::int64_t) &(content.i32);

    case value_type::i64:
        return (std::int64_t) &(content.i64);

    case value_type::u8:
        return (std::int64_t) &(content.u8);

    case value_type::u16:
        return (std::int64_t) &(content.u16);

    case value_type::u32:
        return (std::int64_t) &(content.u32);

    case value_type::u64:
        return (std::int64_t) &(content.u64);

    case value_type::flt:
        return (std::int64_t) &(content.flt);

    case value_type::dbl:
        return (std::int64_t) &(content.dbl);
    }

    return 0;
}


std::string memory_manager::get_value(const value_type p_value_type, const memory_type p_memory_type) {
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


std::uint64_t memory_manager::get_address_sequence() {
    return (std::uint64_t) global_sequence.data();
}


std::vector<std::uint8_t>& memory_manager::get_sequence() {
    return global_sequence;
}


bool memory_manager::change_value(const value_type p_value_type, const memory_type p_memory_type, const change_type p_change_type, const std::string& p_value) {
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
        content.dbl = change(p_change_type, content.dbl, s2v<double>(p_value));
        break;

    default:
        return false;
    }

    return true;
}