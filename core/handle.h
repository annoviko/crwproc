#pragma once

#include <functional>

#include <windows.h>


class handle {
private:
    HANDLE m_handle;

public:
    handle(const HANDLE p_handle);

    handle(const handle& p_other) = delete;

    ~handle();

public:
    HANDLE operator()();
    const HANDLE operator()() const;
};
