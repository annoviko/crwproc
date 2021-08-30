/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once


#include <string>

#include "core/filter.h"

#include "core/console.h"


class filter_reader_value {
public:
    static bool read(filter& p_filter);
    static bool read(filter_equal& p_filter);
    static bool read(filter_range& p_filter);

private:
    template <typename TypeFilter>
    static std::uint64_t get_integral_max_value(const TypeFilter& p_filter) {
        if (p_filter.is_value_signed()) {
            switch (p_filter.get_value_size()) {
                case 1: return std::numeric_limits<std::int8_t>::max();
                case 2: return std::numeric_limits<std::int16_t>::max();
                case 4: return std::numeric_limits<std::int32_t>::max();
                case 8: return std::numeric_limits<std::int64_t>::max();
                default: throw std::invalid_argument("Invalid value size '" + std::to_string(p_filter.get_value_size()) + "' is specified.");
            }
        }
        else {
            switch (p_filter.get_value_size()) {
                case 1: return std::numeric_limits<std::uint8_t>::max();
                case 2: return std::numeric_limits<std::uint16_t>::max();
                case 4: return std::numeric_limits<std::uint32_t>::max();
                case 8: return std::numeric_limits<std::uint64_t>::max();
                default: throw std::invalid_argument("Invalid value size '" + std::to_string(p_filter.get_value_size()) + "' is specified.");
            }
        }
    }


    template <typename TypeFilter>
    static std::int64_t get_integral_min_value(const TypeFilter & p_filter) {
        if (p_filter.is_value_signed()) {
            switch (p_filter.get_value_size()) {
                case 1: return std::numeric_limits<std::int8_t>::min();
                case 2: return std::numeric_limits<std::int16_t>::min();
                case 4: return std::numeric_limits<std::int32_t>::min();
                case 8: return std::numeric_limits<std::int64_t>::min();
                default: throw std::invalid_argument("Invalid value size '" + std::to_string(p_filter.get_value_size()) + "' is specified.");
            }
        }
        
        return 0;
    }


    template <typename TypeFilter>
    static bool is_out_of_range(const std::uint64_t p_value, const TypeFilter& p_filter) {
        if (p_filter.is_value_signed()) {
            const std::int64_t value_signed = static_cast<std::int64_t>(p_value);
            if (value_signed > static_cast<std::int64_t>(get_integral_max_value(p_filter))) {
                return true;
            }

            if (value_signed < get_integral_min_value(p_filter)) {
                return true;
            }

            if ((value_signed > 0) && (p_value > get_integral_max_value(p_filter))) {
                return true;
            }
        }
        else {
            if (p_value > get_integral_max_value(p_filter)) {
                return true;
            }
        }

        return false;
    }


    template <typename TypeFilter>
    static std::string ask_value(const std::string& p_invite_message, const TypeFilter& p_filter) {
        std::cout << p_invite_message;

        std::string value;
        std::cin >> value;

        try {
            switch (p_filter.get_value_type()) {
            case value::type::integral:
                if (is_out_of_range(std::stoull(value), p_filter)) {
                    console::error_and_wait_key("Error: specified value '" + value + "' is out of range (" 
                        + std::to_string(get_integral_min_value(p_filter)) + ", " + std::to_string(get_integral_max_value(p_filter)) + ").");
                    return std::string{ };
                }

                break;


            case value::type::floating:
                std::stof(value);
                break;

            case value::type::doubling:
                std::stod(value);
                break;

            default:
                return std::string{ };
            }
        }
        catch (...) {
            console::error_and_wait_key("Error: invalid value is specified '" + value + "'.");
            return std::string{ };
        }

        return value;
    }
};