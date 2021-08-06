#pragma once


#include <variant>

class event_choose { };

class event_filter { };

class event_update { };

class event_show { };

class event_done { };

class event_error { };

class event_exit { };


using event = std::variant<
    event_choose, 
    event_filter, 
    event_update,
    event_show, 
    event_done, 
    event_error, 
    event_exit
>;
