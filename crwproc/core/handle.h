#pragma once

#include <functional>

#include <windows.h>


class handle {
private:
    HANDLE m_handle;

public:
    handle(const HANDLE p_handle);

    ~handle();

public:
    HANDLE operator()();
    const HANDLE operator()() const;
};
