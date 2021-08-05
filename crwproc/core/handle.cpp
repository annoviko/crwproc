#include "handle.h"


handle::handle(const HANDLE p_handle) : m_handle(p_handle)
{ }


handle::~handle() {
    CloseHandle(m_handle);
}


HANDLE handle::operator()() {
    return m_handle;
}


const HANDLE handle::operator()() const {
    return m_handle;
}