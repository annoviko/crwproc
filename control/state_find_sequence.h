/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once


#include <vector>

#include "state_base.h"


class state_find_sequence : public state_base {
private:
    std::vector<std::uint8_t> m_sequence = { };
    std::uint64_t m_address = 0;

public:
    event operator()(const context& p_context);

private:
    bool ask_byte_sequence();

    bool add_byte(std::string& p_byte);

    void find_byte_sequence(const context& p_context);

    void show_result(const context& p_context) const;

public:
    friend std::ostream& operator<<(std::ostream& p_stream, const state_find_sequence& p_state);
};
