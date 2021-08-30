/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "filter.h"


std::ostream& operator<<(std::ostream& p_stream, const filter& p_filter) {
    std::visit([&p_stream](auto&& instance) {
        p_stream << instance;
    }, p_filter);

    return p_stream;
}