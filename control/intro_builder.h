/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once

#include <string>

#include "core/console.h"
#include "control/context.h"


class intro_builder {
private:
    std::string m_intro;

    position m_begin_position;
    position m_end_position;

public:
    intro_builder() = default;

    explicit intro_builder(const std::string& p_intro);

public:
    void show(const context& p_context);

    void redraw(const context& p_context) const;

public:
    static void show(const context& p_context, const std::string& p_intro);
};