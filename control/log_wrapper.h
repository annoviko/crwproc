/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once


#include "log/logging.h"

#include "console/control.h"


#define LOG_ERROR_WITH_WAIT_KEY(_message) {                                \
    LOG_ERROR(_message)                                                    \
    crwproc::console::control::error_and_wait_key(_message);               \
}


#define LOG_ERROR_WITH_WAIT_KEY_AND_RETURN(_message) {                     \
    LOG_ERROR_WITH_WAIT_KEY(_message)                                      \
    return;                                                                \
}


#define LOG_ERROR_WITH_WAIT_KEY_AND_RETURN_VALUE(_message, _value) {       \
    LOG_ERROR_WITH_WAIT_KEY(_message)                                      \
    return _value;                                                         \
}


#define LOG_WARNING_WITH_WAIT_KEY(_message) {                              \
    LOG_WARNING(_message)                                                  \
    crwproc::console::control::warning_and_wait_key(_message);             \
}


#define LOG_WARNING_WITH_WAIT_KEY_AND_RETURN(_message) {                   \
    LOG_WARNING_WITH_WAIT_KEY(_message)                                    \
    return;                                                                \
}


#define LOG_WARNING_WITH_WAIT_KEY_AND_RETURN_VALUE(_message, _value) {     \
    LOG_WARNING_WITH_WAIT_KEY(_message)                                    \
    return _value;                                                         \
}
