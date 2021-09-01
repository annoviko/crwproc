/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "filter_reader_value.h"


bool filter_reader_value::read(filter& p_filter) {
    bool reading_result = false;
    std::visit([&reading_result, &p_filter](auto&& instance) {
        using FilterType = std::decay_t<decltype(instance)>;
        if constexpr (crwproc::traits::is_any<FilterType, filter_equal, filter_range>::value) {
            reading_result = read_filter(instance);
        }
        else if constexpr (crwproc::traits::is_any<FilterType, filter_more, filter_less>::value) {
            reading_result = read_filter<FilterType>(p_filter);
        }
    }, p_filter);

    return reading_result;
}


bool filter_reader_value::read_filter(filter_equal& p_filter) {
    const std::string value = ask_value("Enter value that is going to be searched in the process: ", p_filter);
    if (value.empty()) {
        return false;
    }

    std::cout << std::endl;
    p_filter.set_value(value);
    return true;
}


bool filter_reader_value::read_filter(filter_range& p_filter) {
    std::string value = ask_value("Enter 'from' value for the range to search in the process: ", p_filter);
    if (value.empty()) {
        return false;
    }

    std::cout << std::endl;
    p_filter.set_begin_value(value);

    value = ask_value("Enter 'to' value for the range to search in the process: ", p_filter);
    if (value.empty()) {
        return false;
    }

    std::cout << std::endl;
    p_filter.set_end_value(value);

    return true;
}
