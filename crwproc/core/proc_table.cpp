#include "proc_table.h"

#include <iostream>

#include <windows.h>
#include <psapi.h>

#include "handle.h"


proc_info proc_table::get_info(const std::size_t p_pid) const
{
    handle proc_handler = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, p_pid);
    if (!proc_handler()) {
        return proc_info();
    }

    HMODULE modules[2048];
    std::memset(modules, 0x00, sizeof(modules));

    DWORD amount_module_bytes = 0;
    if (!EnumProcessModules(proc_handler(), modules, sizeof(modules), &amount_module_bytes)) {
        return proc_info();
    }

    char proc_name[512];
    std::memset(proc_name, 0x00, sizeof(proc_name));

    if (!GetModuleBaseName(proc_handler(), modules[0], proc_name, sizeof(proc_name) / sizeof(char))) {
        return proc_info();
    }

    return proc_info(p_pid, std::string(proc_name));
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