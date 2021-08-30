/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once


#include <variant>
#include <iostream>

#include "filter_equal.h"
#include "filter_none.h"
#include "filter_range.h"


using filter = std::variant<filter_equal, filter_none, filter_range>;


std::ostream& operator<<(std::ostream& p_stream, const filter& p_filter);
