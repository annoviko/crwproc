/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "utils.h"

#include "core/search_result.h"


bool contains_address(const search_result& p_result, const void* p_address) {
    for (const auto& block : p_result.get_memory_blocks()) {
        if (block.is_raw_memory()) {
            if (((std::uint64_t)p_address >= block.get_begin()) && ((std::uint64_t)p_address <= block.get_end())) {
                return true;
            }
        }
        else {
            for (const auto& pointer : block.get_values()) {
                if ((std::uint64_t)p_address == pointer.get_address()) {
                    return true;
                }
            }
        }
    }

    return false;
}
