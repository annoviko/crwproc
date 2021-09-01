/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "ut-proc_reader.h"

#include "core/filter_more.h"


TEST(ut_filter_less, find_value_int8) {
    std::int8_t stack_value = 100;
    test_template_find_value<filter_less>(&stack_value, [&stack_value]() { stack_value--; });
}
