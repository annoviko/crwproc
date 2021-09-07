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

    type_desc type = type_desc::create<TypeValue>();
    proc_pointer pointer = proc_pointer::create((std::uint64_t) p_address, p_next_value);
    proc_writer(collection[pid]).write(pointer, type);

    pointer = proc_pointer::create((std::uint64_t) p_address, TypeValue(0));
    proc_reader(collection[pid]).refresh(type, pointer);

    ASSERT_EQ(p_next_value, pointer.get_value().get<TypeValue>());
}
