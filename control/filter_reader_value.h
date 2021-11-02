/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once


#include <iostream>
#include <string>
#include <vector>

#include "log/logging.h"

#include "core/console.h"
#include "core/filter.h"
#include "core/traits.h"
#include "core/value.h"

#include "asker.h"
#include "command.h"
#include "log_wrapper.h"


class filter_reader_value {
public:
    static bool read(filter& p_filter);

private:
    static bool read_filter(filter_equal& p_filter);
    static bool read_filter(filter_range& p_filter);

    template <typename TypeFilter>
    static typename std::enable_if<crwproc::traits::is_any<TypeFilter, filter_more, filter_less>::value, bool>::type
    read_filter(filter& p_filter) {
        const static std::vector<std::string> directions = { "More (x[t] > x[t-1]", "Less (x[t] < x[t-1]" };

        std::cout << "Choose filtering type: " << std::endl;
        for (std::size_t i = 0; i < directions.size(); i++) {
            console::set_foreground_color(color::blue, true);
            std::cout << " " << i;
            console::set_defaut_color();

            std::cout << " - " << directions[i] << std::endl;
        }

        std::cout << "Enter option number (0-" << directions.size() - 1 << "): ";
        const index_info index_option = asker::ask_index(directions.size(), true);
        if (!index_option.is_valid()) {
            return false;
        }

        const type_desc type = std::visit([](auto&& instance) {
            return instance.get_type();
        }, p_filter);

        switch (index_option.get_begin()) {
        case 0:
            p_filter = filter_more(type);
            break;

        case 1:
            p_filter = filter_less(type);
            break;

        default:
            return false;
        }

        return true;
    }


private:
    template <typename TypeFilter>
    static std::uint64_t get_integral_max_value(const TypeFilter& p_filter) {
        if (p_filter.get_type().is_signed()) {
            switch (p_filter.get_type().get_size()) {
                case 1: return std::numeric_limits<std::int8_t>::max();
                case 2: return std::numeric_limits<std::int16_t>::max();
                case 4: return std::numeric_limits<std::int32_t>::max();
                case 8: return std::numeric_limits<std::int64_t>::max();
                default: throw std::invalid_argument("Invalid value size '" + std::to_string(p_filter.get_type().get_size()) + "' is specified.");
            }
        }
        else {
            switch (p_filter.get_type().get_size()) {
                case 1: return std::numeric_limits<std::uint8_t>::max();
                case 2: return std::numeric_limits<std::uint16_t>::max();
                case 4: return std::numeric_limits<std::uint32_t>::max();
                case 8: return std::numeric_limits<std::uint64_t>::max();
                default: throw std::invalid_argument("Invalid value size '" + std::to_string(p_filter.get_type().get_size()) + "' is specified.");
            }
        }
    }


    template <typename TypeFilter>
    static std::int64_t get_integral_min_value(const TypeFilter & p_filter) {
        if (p_filter.get_type().is_signed()) {
            switch (p_filter.get_type().get_size()) {
                case 1: return std::numeric_limits<std::int8_t>::min();
                case 2: return std::numeric_limits<std::int16_t>::min();
                case 4: return std::numeric_limits<std::int32_t>::min();
                case 8: return std::numeric_limits<std::int64_t>::min();
                default: throw std::invalid_argument("Invalid value size '" + std::to_string(p_filter.get_type().get_size()) + "' is specified.");
            }
        }
        
        return 0;
    }


    template <typename TypeFilter>
    static bool is_out_of_range(const std::uint64_t p_value, const TypeFilter& p_filter) {
        if (p_filter.get_type().is_signed()) {
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

        LOG_INFO("User input (value): '" << value << "'.")

        command::throw_if_command(value);

        try {
            switch (p_filter.get_type().get_type()) {
            case value_type::integral:
                if (is_out_of_range(std::stoull(value), p_filter)) {
                    LOG_ERROR_WITH_WAIT_KEY_AND_RETURN_VALUE("Error: specified value '" + value + "' is out of range ("
                        + std::to_string(get_integral_min_value(p_filter)) + ", " + std::to_string(get_integral_max_value(p_filter)) + ").", std::string{ })
                }

                break;

            case value_type::floating:
                (void) std::stof(value);    /* check if it is possible to parse */
                break;

            case value_type::doubling:
                (void) std::stod(value);    /* check if it is possible to parse */
                break;

            default:
                return std::string{ };
            }
        }
        catch (...) {
            LOG_ERROR_WITH_WAIT_KEY_AND_RETURN_VALUE("Error: invalid value is specified '" + value + "'.", std::string{ })
        }

        return value;
    }
};