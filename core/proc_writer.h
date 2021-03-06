/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once

#include <cstdint>

#include <windows.h>

#include "proc_handle.h"
#include "proc_info.h"
#include "proc_pointer.h"
#include "type_desc.h"


class proc_writer {
private:
    proc_info   m_proc_info;

public:
    explicit proc_writer(const proc_info& p_info);

public:
    bool write(const proc_pointer& p_pointer, const type_desc& p_type) const;

private:
    bool write_integral(const proc_handle& p_handle, const proc_pointer& p_pointer, const type_desc& p_type) const;

    template <typename TypeValue>
    static bool write_value(const proc_handle& p_handle, const std::uint64_t p_address, const TypeValue p_value) {
        if (p_handle.is_running()) {
            return (bool) WriteProcessMemory(p_handle(), (LPVOID)p_address, (LPCVOID) &p_value, sizeof(TypeValue), nullptr);
        }

        return false;
    }
};