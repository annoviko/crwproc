/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "ut-proc_reader.h"

#include "core/filter_more.h"


TEST(ut_filter_more, find_value_int8) {
    std::int8_t stack_value = 0;
    test_template_find_value<filter_more>(&stack_value, [&stack_value]() { stack_value++; });
}
