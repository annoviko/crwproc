#include "proc_writer.h"

#include <memory>
#include <exception>

#include <windows.h>

#include "handle.h"


proc_writer::proc_writer(const proc_info& p_info) :
    m_proc_info(p_info)
{ }


bool proc_writer::write(const proc_pointer& p_pointer) const {
    handle proc_handler = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_WRITE, FALSE, static_cast<DWORD>(m_proc_info.pid()));

    switch (p_pointer.get_value().get_type()) {
    case value::type::integral:
        return write_integral(proc_handler, p_pointer);

    case value::type::floating:
        return write_value(proc_handler, p_pointer.get_address(), std::stof(p_pointer.get_value().get_value()));

    case value::type::doubling:
        return write_value(proc_handler, p_pointer.get_address(), std::stod(p_pointer.get_value().get_value()));
    }

    throw std::logic_error("Impossible to write value to the process memory (reason: unknown value type '" + std::to_string(static_cast<std::uint64_t>(p_pointer.get_value().get_type())) + "' is detected).");
}


bool proc_writer::write_integral(const handle& p_handle, const proc_pointer& p_pointer) const {
    switch (p_pointer.get_value().get_size()) {
    case 1:
        return write_value(p_handle, p_pointer.get_address(), static_cast<std::uint8_t>(std::stoull(p_pointer.get_value().get_value())));

    case 2:
        return write_value(p_handle, p_pointer.get_address(), static_cast<std::uint16_t>(std::stoull(p_pointer.get_value().get_value())));

    case 4:
        return write_value(p_handle, p_pointer.get_address(), static_cast<std::uint32_t>(std::stoull(p_pointer.get_value().get_value())));

    case 8:
        return write_value(p_handle, p_pointer.get_address(), static_cast<std::uint64_t>(std::stoull(p_pointer.get_value().get_value())));
    }

    throw std::logic_error("Impossible to write value to the process memory (reason: incorrect value size '" + std::to_string(p_pointer.get_value().get_size()) + "').");
}