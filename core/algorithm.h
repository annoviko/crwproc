/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once


#include <limits>
#include <vector>


#undef max


namespace crwproc
{

namespace algorithm
{


constexpr static std::size_t npos = std::numeric_limits<std::size_t>::max();


template <typename TypeCollection, typename TypePatternMap>
void kmp_pattern_map(const TypeCollection& p_pattern, const std::size_t p_length, TypePatternMap& p_pattern_map) {
    std::size_t j = 0;
    for (std::size_t i = 1; i < p_length; i++) {
        while((j != 0) && (p_pattern[j] != p_pattern[i])) {
            j = p_pattern_map[j - 1];
        }

        if (p_pattern[i] == p_pattern[j]) {
            j++;
            p_pattern_map[i] = j;
        }
    }
}


template <typename TypeInput, typename TypePattern>
std::size_t kmp(const TypeInput& p_input, const std::size_t p_input_length, const TypePattern& p_pattern, const std::size_t p_pattern_length) {
    std::vector<std::size_t> pattern_map(p_pattern_length, 0);
    kmp_pattern_map(p_pattern, p_pattern_length, pattern_map);

    std::size_t j = 0;
    std::size_t i = 0;
    for (; (i < p_input_length) && (j < p_pattern_length); i++) {
        while((j != 0) && (p_input[i] != p_pattern[j])) {
            j = pattern_map[j - 1];
        }

        if (p_input[i] == p_pattern[j]) {
            j++;
        }
    }

    if (j == p_pattern_length) {
        return i - j;
    }
    else {
        return npos;
    }
}


}

}