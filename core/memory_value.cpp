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


bool memory_value::set(const std::string& p_value, const type_desc& p_type, const std::int64_t p_multiplier) try {
    switch (p_type.get_type()) {
        case value_type::integral: {
            if (p_type.is_signed()) {
                const std::int64_t value = std::stoll(p_value) * p_multiplier;
                std::memcpy((void*)m_buffer, (void*)&value, p_type.get_size());
            }
            else {
                const std::uint64_t value = std::stoull(p_value) * static_cast<std::uint64_t>(p_multiplier);
                std::memcpy((void*)m_buffer, (void*)&value, p_type.get_size());
            }

            break;
        }

        case value_type::floating: {
            const float value = std::stof(p_value) * static_cast<float>(p_multiplier);
            std::memcpy((void*)m_buffer, (void*)&value, p_type.get_size());
            break;
        }

        case value_type::doubling: {
            const double value = std::stod(p_value) * static_cast<double>(p_multiplier);
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



std::string memory_value::to_string(const type_desc& p_type, const std::int64_t p_multiplier) const {
    switch (p_type.get_type()) {
    case value_type::integral:
        switch (p_type.get_size()) {
        case 1:
            return p_type.is_signed() ? std::to_string(*((std::int8_t*)m_buffer) / p_multiplier) : std::to_string(*((std::uint8_t*)m_buffer) / p_multiplier);

        case 2:
            return p_type.is_signed() ? std::to_string(*((std::int16_t*)m_buffer) / p_multiplier) : std::to_string(*((std::uint16_t*)m_buffer) / p_multiplier);

        case 4:
            return p_type.is_signed() ? std::to_string(*((std::int32_t*)m_buffer) / p_multiplier) : std::to_string(*((std::uint32_t*)m_buffer) / p_multiplier);

        case 8:
            return p_type.is_signed() ? std::to_string(*((std::int64_t*)m_buffer) / p_multiplier) : std::to_string(*((std::uint64_t*)m_buffer) / p_multiplier);
        }

    case value_type::floating:
        return std::to_string(*((float*)((void*)m_buffer)) / static_cast<float>(p_multiplier));

    case value_type::doubling:
        return std::to_string(*((double*)((void*)m_buffer)) / static_cast<float>(p_multiplier));

    default:
        return "invalid";
    }
}
