#include "proc_reader.h"

#include <memory>

#include <windows.h>
#include <psapi.h>

#include "handle.h"


proc_reader::proc_reader(const proc_info& p_info) :
    m_proc_info(p_info)
{ }


proc_pointer_sequence proc_reader::read() const {
    proc_pointer_sequence result;

    handle proc_handler = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, m_proc_info.pid());

    if (!proc_handler()) {
        return { };
    }

    std::uint64_t proc_size = get_proc_size(proc_handler);
    if ((proc_size == INVALID_PROC_SIZE) || (proc_size == 0)) {
        return { };
    }

    std::shared_ptr<std::uint8_t[]> buffer(new std::uint8_t[READ_WINDOW_SIZE]);

    std::uint64_t current_base_address = m_proc_info.base_address();
    std::uint64_t remaining_proc_size = proc_size;
    while (remaining_proc_size != 0) {
        std::memset(buffer.get(), 0x00, READ_WINDOW_SIZE);

        std::uint64_t bytes_to_read = remaining_proc_size > READ_WINDOW_SIZE ? READ_WINDOW_SIZE : remaining_proc_size;
        std::uint64_t bytes_was_read = 0;

        if (!ReadProcessMemory(proc_handler(), (LPCVOID) current_base_address, buffer.get(), bytes_to_read, (SIZE_T *) &bytes_was_read)) {
            continue;
        }

        remaining_proc_size -= bytes_was_read;
        current_base_address += bytes_was_read;

        extract_values(buffer.get(), bytes_was_read, result);
    }

    return result;
}


proc_pointer_sequence proc_reader::read(const proc_pointer_sequence& p_values) const {
    proc_pointer_sequence result;

    handle proc_handler = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, m_proc_info.pid());
    if (!proc_handler()) {
        return { };
    }

    std::uint8_t buffer[READ_VALUE_SIZE];

    for (const auto& value : p_values) {
        std::memset(buffer, 0x00, READ_VALUE_SIZE);

        std::uint64_t bytes_to_read = get_value_size();
        std::uint64_t bytes_was_read = 0;

        if (!ReadProcessMemory(proc_handler(), (LPCVOID)value.address, buffer, bytes_to_read, (SIZE_T*)&bytes_was_read)) {
            continue;
        }

        extract_value((const uint8_t*)value.address, result);
    }

    return result;
}


void proc_reader::set_read_observer(const read_observer& p_observer) {
    m_reader_observer = p_observer;
}


void proc_reader::set_filter_observer(const filter_observer& p_observer) {
    m_filter_observer = p_observer;
}


std::uint64_t proc_reader::get_proc_size(const handle& p_handle) const {
    PROCESS_MEMORY_COUNTERS memory_counters;

    if (!GetProcessMemoryInfo(p_handle(), &memory_counters, sizeof(memory_counters))) {
        return INVALID_PROC_SIZE;
    }

    return static_cast<std::uint64_t>(memory_counters.WorkingSetSize);

    return 0;
}


void proc_reader::extract_values(const std::uint8_t* p_buffer, const std::uint64_t p_length, proc_pointer_sequence& p_result) const {
    for (std::uint64_t offset = 0; offset < p_length; offset++) {
        extract_value(p_buffer + offset, p_result);
    }
}


void proc_reader::extract_value(const std::uint8_t* p_buffer, proc_pointer_sequence& p_result) const {
    switch (m_filter.get_type()) {
        case filter_value::type::integral: {
            const std::uint64_t expected_value = std::stoull(m_filter.get_value());
            const std::uint64_t actual_value = extract_integral_value(p_buffer);

            if (expected_value == actual_value) {
                p_result.push_back({ (std::uint64_t)p_buffer, m_filter.get_size(), m_filter.get_type() });
            }

            break;
        }

        case filter_value::type::floating: {
            const float expected_value = std::stof(m_filter.get_value());
            const float actual_value = *((float*)p_buffer);

            if (expected_value == actual_value) {
                p_result.push_back({ (std::uint64_t)p_buffer, m_filter.get_size(), m_filter.get_type() });
            }

            break;
        }

        case filter_value::type::doubling: {
            const double expected_value = std::stof(m_filter.get_value());
            const double actual_value = *((float*)p_buffer);

            if (expected_value == actual_value) {
                p_result.push_back({ (std::uint64_t)p_buffer, m_filter.get_size(), m_filter.get_type() });
            }

            break;
        }
    }
}


std::uint64_t proc_reader::extract_integral_value(const std::uint8_t* p_buffer) const {
    switch (m_filter.get_size()) {
        case 1: return *((std::uint8_t*)p_buffer);
        case 2: return *((std::uint16_t*)p_buffer);
        case 4: return *((std::uint32_t*)p_buffer);
        case 8: return *((std::uint64_t*)p_buffer);
    }
}


std::uint64_t proc_reader::get_value_size() const {
    switch (m_filter.get_type()) {
    case filter_value::type::integral:
        return m_filter.get_size();

    case filter_value::type::floating:
        return sizeof(float);

    case filter_value::type::doubling:
        return sizeof(double);
    }
}


void proc_reader::handle_observers(const std::uint64_t p_read_bytes, const std::uint64_t p_size, const std::uint64_t p_value_found) const {
    if (m_reader_observer) {
        m_reader_observer(p_read_bytes, p_size, p_value_found);
    }

    if (m_filter_observer) {
        m_filter_observer(p_value_found);
    }
}
