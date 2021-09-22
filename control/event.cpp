/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "event.h"


std::ostream& operator<<(std::ostream& p_stream, const event& p_event) {
    std::visit([&p_stream](auto&& instance) {
        p_stream << instance;
    }, p_event);

    return p_stream;
}


std::ostream& operator<<(std::ostream& p_stream, const event_choose& p_event) {
    p_stream << "event_choose";
    return p_stream;
}

std::ostream& operator<<(std::ostream& p_stream, const event_filter& p_event) {
    p_stream << "event_filter";
    return p_stream;
}

std::ostream& operator<<(std::ostream& p_stream, const event_update& p_event) {
    p_stream << "event_update";
    return p_stream;
}

std::ostream& operator<<(std::ostream& p_stream, const event_show& p_event) {
    p_stream << "event_show";
    return p_stream;
}

std::ostream& operator<<(std::ostream& p_stream, const event_add& p_event) {
    p_stream << "event_add";
    return p_stream;
}

std::ostream& operator<<(std::ostream& p_stream, const event_edit& p_event) {
    p_stream << "event_edit";
    return p_stream;
}

std::ostream& operator<<(std::ostream& p_stream, const event_set& p_event) {
    p_stream << "event_set";
    return p_stream;
}

std::ostream& operator<<(std::ostream& p_stream, const event_refresh& p_event) {
    p_stream << "event_refresh";
    return p_stream;
}

std::ostream& operator<<(std::ostream& p_stream, const event_remove& p_event) {
    p_stream << "event_remove";
    return p_stream;
}

std::ostream& operator<<(std::ostream& p_stream, const event_help& p_event) {
    p_stream << "event_help";
    return p_stream;
}

std::ostream& operator<<(std::ostream& p_stream, const event_address& p_event) {
    p_stream << "event_address";
    return p_stream;
}

std::ostream& operator<<(std::ostream& p_stream, const event_done& p_event) {
    p_stream << "event_done";
    return p_stream;
}

std::ostream& operator<<(std::ostream& p_stream, const event_error& p_event) {
    p_stream << "event_error";
    return p_stream;
}

std::ostream& operator<<(std::ostream& p_stream, const event_exit& p_event) {
    p_stream << "event_exit";
    return p_stream;
}
