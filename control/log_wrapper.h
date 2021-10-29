/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once


#include "log/logging.h"

#include "core/console.h"


#define LOG_ERROR_WITH_WAIT_KEY(_message) {                                \
    LOG_ERROR(_message)                                                    \
    console::error_and_wait_key(_message);                                 \
}


#define LOG_ERROR_WITH_WAIT_KEY_AND_RETURN(_message) {                     \
    LOG_ERROR_WITH_WAIT_KEY(_message)                                      \
    return;                                                                \
}


#define LOG_ERROR_WITH_WAIT_KEY_AND_RETURN_VALUE(_message, _value) {       \
    LOG_ERROR_WITH_WAIT_KEY(_message)                                      \
    return _value;                                                         \
}