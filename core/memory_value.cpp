/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "memory_value.h"


memory_value::memory_value() {
    std::memset((void*)m_buffer, 0x00, sizeof(m_buffer));
}


memory_value::memory_value(const void* p_buffer, const std::size_t p_size) {
    std::memcpy((void*)m_buffer, p_buffer, p_size);
}


const void* memory_value::get() const {
    return (void*)m_buffer;
}


void memory_value::set(const void* p_buffer, const std::size_t p_size) {
    std::memcpy((void*)m_buffer, p_buffer, p_size);
}


bool memory_value::set(const std::string& p_value, const type_desc& p_type) try {
    switch (p_type.get_type()) {
        case value_type::integral: {
            if (p_type.is_signed()) {
                const std::int64_t value = std::stoll(p_value);
                std::memcpy((void*)m_buffer, (void*)&value, p_type.get_size());
            }
            else {
                const std::uint64_t value = std::stoull(p_value);
                std::memcpy((void*)m_buffer, (void*)&value, p_type.get_size());
            }

            break;
        }

        case value_type::floating: {
            const float value = std::stof(p_value);
            std::memcpy((void*)m_buffer, (void*)&value, p_type.get_size());
            break;
        }

        case value_type::doubling: {
            const double value = std::stod(p_value);
            std::memcpy((void*)m_buffer, (void*)&value, p_type.get_size());
            break;
        }

        default:
            return false;
    }

    return true;
}
catch(std::exception&) {
    return false;
}



std::string memory_value::to_string(const type_desc& p_type) const {
    switch (p_type.get_type()) {
    case value_type::integral:
        switch (p_type.get_size()) {
        case 1:
            return p_type.is_signed() ? std::to_string(*((std::int8_t*)m_buffer)) : std::to_string(*((std::uint8_t*)m_buffer));

        case 2:
            return p_type.is_signed() ? std::to_string(*((std::int16_t*)m_buffer)) : std::to_string(*((std::uint16_t*)m_buffer));

        case 4:
            return p_type.is_signed() ? std::to_string(*((std::int32_t*)m_buffer)) : std::to_string(*((std::uint32_t*)m_buffer));

        case 8:
            return p_type.is_signed() ? std::to_string(*((std::int64_t*)m_buffer)) : std::to_string(*((std::uint64_t*)m_buffer));
        }

    case value_type::floating:
        return p_type.is_signed() ? std::to_string(*((float*)m_buffer)) : std::to_string(*((float*)m_buffer));

    case value_type::doubling:
        return p_type.is_signed() ? std::to_string(*((double*)m_buffer)) : std::to_string(*((double*)m_buffer));

    default:
        return "invalid";
    }
}