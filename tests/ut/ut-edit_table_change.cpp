/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include <gtest/gtest.h>

#include <windows.h>

#include "core/proc_table.h"

#include "control/edit_table_change.h"


TEST(ut_edit_table_change, revert) {
    std::size_t pid = static_cast<std::size_t>(GetCurrentProcessId());
    proc_info info = proc_table().get().at(pid);

    std::int8_t i8 = 8;
    std::int16_t i16 = 16;

    edit_table_change change_table(info);
    edit_table table = {
        edit_table_entry{ "oil", proc_pointer((std::uint64_t) &i8), type_desc::create<std::int8_t>() },
        edit_table_entry{ "wood", proc_pointer((std::uint64_t) &i16), type_desc::create<std::int16_t>() }
    };

    for (const auto& entry : table) {
        change_table.add(entry);
    }

    ASSERT_EQ(table.size(), change_table.size());

    i8 = 16;
    i16 = 32;

    change_table.revert();
    ASSERT_EQ(8, i8);
    ASSERT_EQ(16, i16);
}