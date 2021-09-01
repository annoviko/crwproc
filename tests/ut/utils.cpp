/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "utils.h"


bool contains_address(const proc_pointer_sequence& p_seq, const void* p_address) {
    for (const auto& p_entry : p_seq) {
        if ((std::uint64_t)p_address == p_entry.get_address()) {
            return true;
        }
    }

    return false;
}
