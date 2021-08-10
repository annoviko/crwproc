#include "proc_reader.h"

#include <exception>
#include <memory>

#include <windows.h>
#include <psapi.h>
#include <tlhelp32.h>

#include "handle.h"


proc_reader::proc_reader(const proc_info& p_info, const filter_equal& p_filter) :
    m_proc_info(p_info),
    m_filter(p_filter)
{ }


proc_pointer_sequence proc_reader::read_and_filter() const {
    proc_pointer_sequence result;

    handle proc_handler = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, static_cast<DWORD>(m_proc_info.pid()));

    m_bytes_to_read = get_amount_bytes_to_read(proc_handler);
    m_bytes_read = 0;
    run_notifier();

    MEMORY_BASIC_INFORMATION  memory_info;
    std::uint64_t current_address = 0;

    while (VirtualQueryEx(proc_handler(), (LPCVOID)current_address, &memory_info, sizeof(memory_info))) {
        if ((memory_info.State == MEM_COMMIT) && ((memory_info.Type == MEM_MAPPED) || (memory_info.Type == MEM_PRIVATE))) {
            std::shared_ptr<std::uint8_t[]> buffer(new std::uint8_t[memory_info.RegionSize]);
            std::memset(buffer.get(), 0x00, memory_info.RegionSize);

            std::uint64_t bytes_was_read = 0;

            if (ReadProcessMemory(proc_handler(), (LPCVOID)memory_info.BaseAddress, buffer.get(), memory_info.RegionSize, (SIZE_T*)&bytes_was_read)) {
                extract_values(buffer.get(), bytes_was_read, (std::uint64_t)memory_info.BaseAddress, m_filter.get_value(), true, result);
            }
        }

        current_address += memory_info.RegionSize;
    }

    stop_notifier();
    return result;
}


proc_pointer_sequence proc_reader::read_and_filter(const proc_pointer_sequence& p_values) const {
    proc_pointer_sequence result;
    result.reserve(p_values.size());

    m_bytes_to_read = p_values.size() * m_filter.get_value().get_size();
    m_bytes_read = 0;
    run_notifier();

    handle proc_handler = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, static_cast<DWORD>(m_proc_info.pid()));

    for (const auto& value : p_values) {
        proc_pointer pointer = read_value(proc_handler, value.get_address(), value.get_value());

        if (m_filter.is_satisfying(pointer.get_value().get<std::string>())) {
            result.push_back(value);
        }

        m_bytes_read += value.get_value().get_size();
    }

    stop_notifier();
    return result;
}


proc_pointer_sequence proc_reader::read(const proc_pointer_sequence& p_values) const {
    proc_pointer_sequence result;
    result.reserve(p_values.size());

    handle proc_handler = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, static_cast<DWORD>(m_proc_info.pid()));

    for (const auto& pointer : p_values) {
        result.push_back(read_value(proc_handler, pointer.get_address(), pointer.get_value()));
    }

    return result;
}


void proc_reader::subscribe(const progress_observer& p_observer) {
    m_observer = p_observer;
}


std::uint64_t proc_reader::get_proc_size(const std::uint64_t p_pid) const {
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, static_cast<DWORD>(p_pid));
    std::uint64_t proc_size = INVALID_PROC_SIZE;

    if (snapshot != INVALID_HANDLE_VALUE) {
        MODULEENTRY32 module_info = { 0 };
        module_info.dwSize = sizeof(MODULEENTRY32);

        if (Module32First(snapshot, &module_info)) {
            proc_size = module_info.modBaseSize;
        }

        CloseHandle(snapshot);
    }

    return proc_size;
}


proc_pointer proc_reader::read_value(const handle& p_proc_handler, const std::uint64_t p_address, const value& p_value) const {
    std::uint8_t buffer[READ_VALUE_SIZE];
    std::memset(buffer, 0x00, READ_VALUE_SIZE);

    std::uint64_t bytes_to_read = p_value.get_size();
    std::uint64_t bytes_was_read = 0;

    if (!ReadProcessMemory(p_proc_handler(), (LPCVOID)p_address, buffer, bytes_to_read, (SIZE_T*)&bytes_was_read)) {
        return { };
    }

    return extract_value(buffer, p_address, p_value);
}


void proc_reader::extract_values(const std::uint8_t* p_buffer, const std::uint64_t p_length, const std::uint64_t p_address, const value& p_value, const bool p_filter, proc_pointer_sequence& p_result) const {
    for (std::uint64_t offset = 0; offset < p_length; offset++) {
        proc_pointer value = extract_value(p_buffer + offset, p_address + offset, p_value);

        m_bytes_read += p_value.get_size();
        if (p_filter && m_filter.is_satisfying(value.get_value().get<std::string>())) {
            p_result.push_back(value);
        }
    }
}


proc_pointer proc_reader::extract_value(const std::uint8_t* p_buffer, const std::uint64_t p_address, const value& p_value) const {
    switch (p_value.get_type()) {
        case value::type::integral: {
            const std::uint64_t actual_value = extract_integral_value(p_buffer, p_value.get_size());
            return { p_address, value(p_value.get_type(), p_value.get_size(), std::to_string(actual_value)) };
        }

        case value::type::floating: {
            const float actual_value = *((float*)p_buffer);
            return { p_address, value(p_value.get_type(), p_value.get_size(), std::to_string(actual_value)) };
        }

        case value::type::doubling: {
            const double actual_value = *((double*)p_buffer);
            return { p_address, value(p_value.get_type(), p_value.get_size(), std::to_string(actual_value)) };
        }

        default:
            throw std::logic_error("Unknown value type '" + std::to_string(static_cast<std::size_t>(p_value.get_type())) + "'.");
    }
}


std::uint64_t proc_reader::extract_integral_value(const std::uint8_t* p_buffer, const std::size_t p_size) const {
    switch (p_size) {
        case 1: return *((std::uint8_t*)p_buffer);
        case 2: return *((std::uint16_t*)p_buffer);
        case 4: return *((std::uint32_t*)p_buffer);
        case 8: return *((std::uint64_t*)p_buffer);
    }

    return INVALID_INTEGER_VALUE;
}


std::uint64_t proc_reader::get_amount_bytes_to_read(const handle& p_handle) const {
    MEMORY_BASIC_INFORMATION  memory_info;

    std::uint64_t current_address = 0;
    std::uint64_t bytes_to_process = 0;

    while (VirtualQueryEx(p_handle(), (LPCVOID)current_address, &memory_info, sizeof(memory_info))) {
        if ((memory_info.State == MEM_COMMIT) && ((memory_info.Type == MEM_MAPPED) || (memory_info.Type == MEM_PRIVATE))) {
            bytes_to_process += memory_info.RegionSize;
        }

        current_address += memory_info.RegionSize;
    }

    return bytes_to_process;
}


std::uint64_t proc_reader::get_progress() const {
    if (m_bytes_read != 0) {
        return m_bytes_read * 100 / m_bytes_to_read;
    }

    return 0;
}


void proc_reader::run_notifier() const {
    if (m_observer) {
        m_stop_notifier = false;
        m_notifer = std::thread(&proc_reader::notifier_thread, this);
    }
}


void proc_reader::stop_notifier() const {
    if (!m_notifer.joinable()) {
        return;
    }

    {
        std::unique_lock<std::mutex> guard(m_event_mutex);
        m_stop_notifier = true;
    }

    m_event.notify_one();
    m_notifer.join();
}


void proc_reader::notifier_thread() const {
    std::unique_lock<std::mutex> guard(m_event_mutex);

    while (!m_stop_notifier) {
        m_observer(get_progress());
        m_event.wait_for(guard, std::chrono::milliseconds(NOTIFICATION_PERIOD_MS), [this]() { return m_stop_notifier; });
    }

    m_observer(100);
}
