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
    proc_handle(const std::size_t p_pid, const access p_access);

    proc_handle(const proc_handle&) = delete;

    ~proc_handle();

public:
    bool is_running() const;

public:
    HANDLE operator()() const;

public:
    proc_handle& operator=(const proc_handle& other) = delete;
};
