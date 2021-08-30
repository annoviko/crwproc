/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once


#include <gtest/gtest.h>

#include <cstddef>
#include <functional>
#include <string>
#include <thread>

#include <windows.h>

#include "core/filter.h"
#include "core/proc_table.h"
#include "core/proc_reader.h"


using changer = std::function<void(void)>;


bool contains_address(const proc_pointer_sequence& p_seq, const void* p_address) {
    for (const auto& p_entry : p_seq) {
        if ((std::uint64_t) p_address == p_entry.get_address()) {
            return true;
        }
    }

    return false;
}


template <typename TypeValue>
void test_template_find_value(TypeValue* p_value, const changer& p_func) {
    std::size_t pid = static_cast<std::size_t>(GetCurrentProcessId());
    proc_info info = proc_table().get().at(pid);

    filter_equal filter(value::create(*p_value));
    auto result = proc_reader(info, filter).read_and_filter();

    ASSERT_FALSE(result.empty());

    /* In case of heap the operation system might not be ready to provide information about memory block allocations, need to wait when these information is going to available. */
    const static std::size_t attempts = 20;
    for (std::size_t i = 0; i < attempts && !contains_address(result, p_value); i++) {
        result = proc_reader(info, filter).read_and_filter();
    }

    ASSERT_TRUE(contains_address(result, p_value));

    const static std::size_t attempts_limit = 10;
    for (std::size_t i = 0; (i < attempts_limit) && (result.size() > 1); i++) {
        p_func();
        filter_equal updated_filter = filter_equal(value::create(*p_value));
        result = proc_reader(info, updated_filter).read_and_filter(result);
    }

    ASSERT_FALSE(result.empty());

    int index_value = -1;
    for (std::size_t i = 0; i < result.size(); i++) {
        if (result[i].get_address() == (std::uint64_t)p_value) {
            index_value = (int)i;
            break;
        }
    }

    ASSERT_NE(-1, index_value);
    ASSERT_EQ((std::uint64_t) p_value, result[index_value].get_address());
    ASSERT_EQ(*p_value, result[index_value].get_value().get<TypeValue>());
}