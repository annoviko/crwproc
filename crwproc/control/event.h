#pragma once


#include <variant>

class event_choose { };

class event_filter { };

class event_update { };

class event_show { };

class event_add { };

class event_edit { };

class event_set { };

class event_refresh { };

class event_remove { };

class event_done { };

class event_error { };

class event_exit { };


using event = std::variant<
    event_choose, 
    event_filter, 
    event_update,
    event_show, 
    event_add, 
    event_edit, 
    event_set, 
    event_refresh, 
    event_remove, 
    event_done, 
    event_error, 
    event_exit
>;
