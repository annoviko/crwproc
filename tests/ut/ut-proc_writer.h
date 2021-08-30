/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/

#pragma once


#include <gtest/gtest.h>

#include "core/filter.h"
#include "core/proc_reader.h"
#include "core/proc_table.h"
#include "core/proc_writer.h"


template <typename TypeValue>
void test_template_write_value(void* p_address, TypeValue p_next_value) {
    std::size_t pid = static_cast<std::size_t>(GetCurrentProcessId());

    proc_collection collection = proc_table().get();
    ASSERT_TRUE(collection.count(pid));

    proc_pointer pointer((std::uint64_t)p_address, value::create(p_next_value));

    proc_writer(collection[pid]).write(pointer);

    proc_pointer_sequence proc_ptrs = proc_reader(collection[pid], filter_none{ }).read({ pointer });

    ASSERT_FALSE(proc_ptrs.empty());
    ASSERT_EQ(1, proc_ptrs.size());
    ASSERT_EQ(p_next_value, proc_ptrs[0].get_value().get<TypeValue>());
}
