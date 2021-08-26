#include "state_search.h"

#include <chrono>
#include <iostream>
#include <unordered_map>

#include "core/console.h"
#include "core/proc_reader.h"

#include "command.h"
#include "intro_builder.h"


event state_search::operator()(context& p_context) {
    intro_builder::show(p_context, "Run search process, please wait...");

    std::size_t reported_progress = 0;
    proc_reader reader(p_context.get_proc_info(), p_context.get_filter());
    auto observer = [&reported_progress](std::size_t p_progress) {
        std::size_t bars_to_display = (p_progress - reported_progress) / 2;
        if (bars_to_display != 0) {
            reported_progress = p_progress;
            std::cout << std::string(bars_to_display, char(219));
        }
    };

    reader.subscribe(observer);
    std::cout << "Progress: ";

    if (p_context.get_found_values().empty()) {
        const auto start_time = std::chrono::system_clock::now();
        p_context.get_found_values() = reader.read_and_filter();
        const auto end_time = std::chrono::system_clock::now();

        std::chrono::duration<double> processing_time = end_time - start_time;
        std::cout << std::endl << "Processing time: " << processing_time.count() << " seconds.";
    }
    else {
        p_context.get_found_values() = reader.read_and_filter(p_context.get_found_values());
    }

    std::cout << std::endl;

    if (p_context.get_found_values().empty()) {
        console::warning_and_wait_key("Nothing has been found, please change filter.");
        return event_filter{};
    }

    std::cout << "Amount of found values: " << p_context.get_found_values().size() << "." << std::endl;

    return ask_next_action(p_context);
}