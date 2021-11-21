/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once


#include <cctype>
#include <iterator>


namespace crwproc {

namespace string {

namespace utils {


template <typename TypeSequence>
void trim_left(TypeSequence& p_sequence) {
    using value_type = typename TypeSequence::value_type;
    auto cursor = std::find_if(p_sequence.begin(), p_sequence.end(), [](value_type& p_value) { return !std::isspace(p_value); });
    p_sequence.erase(p_sequence.begin(), cursor);
}


template <typename TypeSequence>
void trim_right(TypeSequence& p_sequence) {
    using value_type = typename TypeSequence::value_type;
    auto cursor = std::find_if(p_sequence.rbegin(), p_sequence.rend(), [](value_type& p_value) { return !std::isspace(p_value); });
    p_sequence.erase(cursor.base(), p_sequence.end());
}


template <typename TypeSequence>
void trim(TypeSequence& p_sequence) {
    trim_right(p_sequence);
    trim_left(p_sequence);
}


}

}

}