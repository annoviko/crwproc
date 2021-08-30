/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once


#include <unordered_map>

#include "proc_info.h"


using proc_collection = std::unordered_map<std::size_t, proc_info>;


class proc_table {
public:
    proc_collection get() const;

private:
    proc_info get_info(const std::size_t p_pid) const;
};