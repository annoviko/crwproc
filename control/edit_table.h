/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once


#include <string>
#include <vector>
#include <unordered_set>

#include "edit_table_entry.h"


using edit_table = std::vector<edit_table_entry>;

using edit_table_names = std::unordered_set<std::string>;
