/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "state.h"


std::ostream& operator<<(std::ostream& p_stream, const state& p_state) {
    std::visit([&p_stream](auto&& instance) {
        p_stream << instance;
    }, p_state);

    return p_stream;
}
