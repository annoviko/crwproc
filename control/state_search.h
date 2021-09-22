/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once

#include <functional>
#include <vector>
#include <unordered_map>

#include "context.h"
#include "event.h"
#include "state_base.h"


class state_search : public state_base {
public:
    event operator()(context& p_context);

public:
    friend std::ostream& operator<<(std::ostream& p_stream, const state_search& p_state);
};
