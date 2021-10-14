#pragma once


#include <string>


enum class memory_type {
    global = 0,
    heap,
    stack,
    invalid
};


memory_type get_memory_type_from_string(const std::string& p_type);
