#include "state_search.h"

#include <iostream>

#include "core/proc_reader.h"


event state_search::operator()(context& p_context) {
    show_intro(p_context);

    proc_reader reader(p_context.get_proc_info());
    auto observer = [](std::uint64_t p_read_bytes, std::uint64_t p_size, std::uint64_t p_found_values) {
        std::cout << "Processed bytes: " << p_read_bytes << "/" << p_size << " (found values: " << p_found_values << ")." << std::endl;
    };

    reader.set_read_observer(observer);
    p_context.set_values(reader.read());

    if (p_context.get_values().empty()) {
        std::cout << "Nothing has been found, please change filter" << std::endl;
        return event_filter{};
    }

    std::cout << "Amount of found values: " << p_context.get_values().size() << ". Do you want to continue filtering process (Y/N)?" << std::endl;
    char user_command = 0;

    return event_exit{};
}


void state_search::show_intro(const context& p_context) const {
    std::cout << p_context << std::endl;
    std::cout << "Run search process, please wait..." << std::endl;
}