/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once

#include <functional>

#include <windows.h>


class proc_handle {
public:
    enum class access {
        read,
        write
    };

private:
    HANDLE m_handle = nullptr;

public:
    proc_handle(const std::size_t p_handle, const access p_access);

    ~proc_handle();

public:
    HANDLE operator()() const;
};
