/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include <gtest/gtest.h>

#include <cstddef>

#include <windows.h>

#include "core/proc_table.h"


TEST(ut_proc_table, find_myself) {
    proc_collection collection = proc_table().get();

    ASSERT_FALSE(collection.empty());
    ASSERT_GT(collection.size(), 1);

    std::size_t pid = static_cast<std::size_t>(GetCurrentProcessId());

    ASSERT_EQ(1, collection.count(pid));
}


TEST(ut_proc_table, find_nothing) {
    proc_collection collection = proc_table().get();

    ASSERT_FALSE(collection.empty());
    ASSERT_GT(collection.size(), 1);
    ASSERT_EQ(0, collection.count(100));
}