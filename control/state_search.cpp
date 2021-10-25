/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "state_search.h"

#include <chrono>
#include <iostream>
#include <unordered_map>

#include "core/console.h"
#include "core/proc_reader.h"

#include "log/logging.h"

#include "command.h"
#include "intro_builder.h"


event state_search::operator()(context& p_context) {
    intro_builder::show(p_context, "Run search process, please wait...");
    LOG_INFO("Run search process.");

    std::visit([&p_context](auto&& filter) {
        std::size_t reported_progress = 0;
        proc_reader reader(p_context.get_proc_info(), filter);
        auto observer = [&reported_progress](std::size_t p_progress) {
            if (p_progress > 100) {
                return;
            }

            const std::size_t bars_to_display = (p_progress - reported_progress) / 2;
            if (bars_to_display != 0) {
                reported_progress = p_progress;
                std::cout << std::string(bars_to_display, '=');
            }
        };

        reader.subscribe(observer);
        std::cout << "Progress: ";

        if (p_context.get_found_values().is_empty()) {
            const auto start_time = std::chrono::system_clock::now();
            p_context.get_found_values() = reader.read_and_filter();
            const auto end_time = std::chrono::system_clock::now();

            std::chrono::duration<double> processing_time = end_time - start_time;
            std::cout << std::endl << "Processing time: " << processing_time.count() << " seconds.";
            LOG_INFO("Processing time: " << processing_time.count() << " seconds.");
        }
        else {
            const auto start_time = std::chrono::system_clock::now();
            p_context.get_found_values() = reader.read_and_filter(p_context.get_found_values());
            const auto end_time = std::chrono::system_clock::now();

            std::chrono::duration<double> processing_time = end_time - start_time;
            std::cout << std::endl << "Processing time: " << processing_time.count() << " seconds.";
            LOG_INFO("Processing time: " << processing_time.count() << " seconds.");
        }

        std::cout << std::endl;
    }, p_context.get_filter());

    if (p_context.get_found_values().is_empty()) {
        const std::string message = "Nothing has been found, please change filter.";

        LOG_INFO(message);
        console::warning_and_wait_key("Nothing has been found, please change filter.");

        return event_filter{};
    }

    const std::string message = "Amount of found values: " + std::to_string(p_context.get_found_values().get_amount_values()) + ".";
    std::cout << message << std::endl;

    LOG_INFO(message)

    return ask_next_action(p_context);
}


std::ostream& operator<<(std::ostream& p_stream, const state_search&) {
    p_stream << "state_search";
    return p_stream;
}
