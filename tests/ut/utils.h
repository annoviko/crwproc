/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/

#pragma once


#include "core/proc_pointer.h"
#include "core/search_result.h"


bool contains_address(const search_result& p_seq, const void* p_address);
