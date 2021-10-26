/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once


#include <exception>

#include "event.h"


class command_interrupt : public std::exception {
private:
    static constexpr char WHAT_MESSAGE[] = "Control command interrupt";

private:
    event m_event;

public:
    command_interrupt() = delete;

    explicit command_interrupt(const event& p_event);

public:
    const char* what() const noexcept override;

    event get() const;
};