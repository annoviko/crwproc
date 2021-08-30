/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "proc_writer.h"

#include <memory>
#include <exception>

#include <windows.h>

#include "proc_handle.h"


proc_writer::proc_writer(const proc_info& p_info) :
    m_proc_info(p_info)
{ }


bool proc_writer::write(const proc_pointer& p_pointer) const {
    proc_handle proc_handler(m_proc_info.pid(), proc_handle::access::write);

    switch (p_pointer.get_value().get_type()) {
    case value::type::integral:
        return write_integral(proc_handler, p_pointer);

    case value::type::floating:
        return write_value(proc_handler, p_pointer.get_address(), p_pointer.get_value().get<float>());

    case value::type::doubling:
        return write_value(proc_handler, p_pointer.get_address(), p_pointer.get_value().get<double>());
    }

    throw std::logic_error("Impossible to write value to the process memory (reason: unknown value type '" + std::to_string(static_cast<std::uint64_t>(p_pointer.get_value().get_type())) + "' is detected).");
}


bool proc_writer::write_integral(const proc_handle& p_handle, const proc_pointer& p_pointer) const {
    switch (p_pointer.get_value().get_size()) {
    case 1:
        return write_value(p_handle, p_pointer.get_address(), p_pointer.get_value().get<std::uint8_t>());

    case 2:
        return write_value(p_handle, p_pointer.get_address(), p_pointer.get_value().get<std::uint16_t>());

    case 4:
        return write_value(p_handle, p_pointer.get_address(), p_pointer.get_value().get<std::uint32_t>());

    case 8:
        return write_value(p_handle, p_pointer.get_address(), p_pointer.get_value().get<std::uint64_t>());
    }

    throw std::logic_error("Impossible to write value to the process memory (reason: incorrect value size '" + std::to_string(p_pointer.get_value().get_size()) + "').");
}
