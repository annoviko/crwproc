/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "proc_table.h"

#include <iostream>

#include <windows.h>
#include <psapi.h>

#include "proc_handle.h"


proc_info proc_table::get_info(const std::size_t p_pid) const
{
    proc_handle proc_handler(p_pid, proc_handle::access::read);
    if (!proc_handler()) {
        return proc_info();
    }

    HMODULE modules[128];
    std::memset(modules, 0x00, sizeof(modules));

    DWORD amount_module_bytes = 0;
    if (!EnumProcessModules(proc_handler(), modules, sizeof(modules), &amount_module_bytes)) {
        return proc_info();
    }

    char proc_name[512];
    std::memset(proc_name, 0x00, sizeof(proc_name));

    HMODULE process_base_address = modules[0];
    if (!GetModuleBaseName(proc_handler(), process_base_address, proc_name, sizeof(proc_name) / sizeof(char))) {
        return proc_info();
    }

    return proc_info(p_pid, std::string(proc_name), (uint64_t)process_base_address);
}


proc_collection proc_table::get() const {
    DWORD process_ids[2048];
    std::memset(process_ids, 0x00, sizeof(process_ids));

    DWORD amount_procs_bytes = 0;
    if (!EnumProcesses(process_ids, sizeof(process_ids), &amount_procs_bytes)) {
        return { };
    }

    const DWORD amount_procs = amount_procs_bytes / sizeof(DWORD);

    proc_collection collection;
    for (std::size_t i = 0; i < amount_procs; i++) {
        const std::size_t pid = process_ids[i];
        const proc_info info = get_info(pid);

        if (info.is_valid()) {
            collection[pid] = info;
        }
    }

    return collection;
}
