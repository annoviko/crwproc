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
#include "core/search_result.h"
#include "core/proc_table.h"
#include "core/proc_reader.h"

#include "utils.h"


using changer = std::function<void(void)>;


template <typename TypeFilter, typename TypeValue>
void test_template_find_value(TypeValue* p_value, const changer& p_func) {
    //std::cout << "Address:     " << (void*)p_value << " - " << (void*)p_value << std::endl;

    std::size_t pid = static_cast<std::size_t>(GetCurrentProcessId());
    proc_info info = proc_table().get().at(pid);

    type_desc type = type_desc::create<TypeValue>();
    TypeFilter filter(type);
    filter.set_value(*p_value);
    search_result result = proc_reader(info, filter).read_and_filter();

    /* In case of heap the operation system might not be ready to provide information about memory block allocations, need to wait when these information is going to available. */
    const static std::size_t attempts = 30;
    for (std::size_t i = 0; i < attempts && !contains_address(result, p_value); i++) {
        result = proc_reader(info, filter).read_and_filter();
    }

    ASSERT_FALSE(result.is_empty());
    ASSERT_TRUE(contains_address(result, p_value));

    const static std::size_t attempts_limit = 10;

    for (std::size_t i = 0; (i < attempts_limit) && (result.get_amount_values() > 1); i++) {
        p_func();

        TypeFilter updated_filter = TypeFilter(type);
        updated_filter.set_value(*p_value);

        result = proc_reader(info, updated_filter).read_and_filter(result);

        ASSERT_TRUE(contains_address(result, p_value));
    }

    ASSERT_FALSE(result.is_empty());

    for (const auto& block : result.get_memory_blocks()) {
        for (const auto& pointer : block.get_values()) {
            if (pointer.get_address() == (std::uint64_t)p_value) {
                ASSERT_EQ(*p_value, pointer.get_value().get<TypeValue>());
                return;
            }
        }
    }

    FAIL() << "Value is not found";
}
