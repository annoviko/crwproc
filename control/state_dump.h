/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once


#include <optional>
#include <memory>

#include "state_base.h"


class state_dump : public state_base {
private:
    std::optional<std::uint64_t> m_address = { };
    std::optional<std::size_t> m_length = { };
    std::shared_ptr<std::uint8_t[]> m_dump = nullptr;

public:
    event operator()(const context& p_context);

private:
    void ask_dump_info();

    bool read_dump(const proc_info& p_info);

    void show_dump(const context& p_context) const;

public:
    friend std::ostream& operator<<(std::ostream& p_stream, const state_dump& p_state);
};
