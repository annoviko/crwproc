/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/

#pragma once


#include <iterator>
#include <string>


namespace crwproc {

namespace string
{


template <typename InsertIterator>
void split(const std::string& p_message, InsertIterator p_inserter) {
    std::size_t begin = 0;
    std::size_t end = 0;
    bool is_state_word = false;

    for (std::size_t i = 0; i < p_message.size(); i++) {
        if (p_message[i] != ' ' && p_message[i] != '\n') {
            if (!is_state_word) {
                begin = i;
            }

            end = i;
            is_state_word = true;
        }
        else {
            if (begin != end) {
                p_inserter = p_message.substr(begin, end - begin + 1);
            }

            begin = end;
            is_state_word = false;
        }
    }

    if (is_state_word) {
        const std::string last_word = p_message.substr(begin);
        if (!last_word.empty()) {
            p_inserter = last_word;
        }
    }
}


}

}