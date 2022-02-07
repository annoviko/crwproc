/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "state_search.h"

#include <chrono>
#include <iostream>

#include "core/console.h"
#include "core/console_progress_bar.h"
#include "core/proc_reader.h"

#include "log/logging.h"

#include "command.h"
#include "intro_builder.h"
#include "log_wrapper.h"


event state_search::operator()(context& p_context) {
    m_intro = intro_builder("Run search process, please wait...");
    m_intro.show(p_context);

    LOG_INFO("Run search process using filter '" << p_context.get_filter() << "'.")

    console_progress_bar progress_control(60);
    progress_control.display();

    std::visit([&p_context, &progress_control](auto&& filter) {
        std::size_t reported_progress = 0;
        proc_reader reader(p_context.get_proc_info(), filter);
        auto observer = [&reported_progress, &progress_control](std::size_t p_progress) {
            if (p_progress > 100) {
                return;
            }

            progress_control.update(p_progress);
        };

        reader.subscribe(observer);

        if (p_context.get_found_values().is_empty()) {
            const auto start_time = std::chrono::system_clock::now();
            p_context.get_found_values() = reader.read_and_filter();
            const auto end_time = std::chrono::system_clock::now();

            const std::chrono::duration<double> processing_time = end_time - start_time;
            std::cout << std::endl << "Processing time: " << processing_time.count() << " seconds.";
            LOG_INFO("Processing time: " << processing_time.count() << " seconds.")
        }
        else {
            const auto start_time = std::chrono::system_clock::now();
            p_context.get_found_values() = reader.read_and_filter(p_context.get_found_values());
            const auto end_time = std::chrono::system_clock::now();

            const std::chrono::duration<double> processing_time = end_time - start_time;
            std::cout << std::endl << "Processing time: " << processing_time.count() << " seconds.";
            LOG_INFO("Processing time: " << processing_time.count() << " seconds.")
        }

        std::cout << std::endl;
    }, p_context.get_filter());

    if (p_context.get_found_values().is_empty()) {
        LOG_ERROR_WITH_WAIT_KEY_AND_RETURN_VALUE("Nothing has been found, please change filter.", event_filter{})
    }

    const std::string message = "Amount of found values: " + std::to_string(p_context.get_found_values().get_amount_values()) + ".";
    std::cout << message << std::endl;

    m_intro.redraw(p_context);

    LOG_INFO(message)

    event action = continue_until_event_is_not<event_error>([&action, &p_context]() {
        return ask_next_action(p_context);
    });

    return action;
}


std::ostream& operator<<(std::ostream& p_stream, const state_search&) {
    p_stream << "state_search";
    return p_stream;
}
