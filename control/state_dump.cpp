/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "state_dump.h"

#include <iomanip>

#include "asker.h"
#include "intro_builder.h"
#include "log_wrapper.h"

#include "core/proc_reader.h"


event state_dump::operator()(const context& p_context) {
    intro_builder::show(p_context, "Create a memory view at the specific address.");

    ask_dump_info();
    if (!read_dump(p_context.get_proc_info())) {
        return event_error{ };
    }

    return continue_until_event_is_not<event_refresh>([this, &p_context]() {
        show_dump(p_context);
        return ask_next_action(p_context);
    });
}


void state_dump::ask_dump_info() {
    while (!m_address.has_value()) {
        m_address = asker::ask_address();
    }

    while (!m_length.has_value()) {
        m_length = asker::ask_length();
    }
}


bool state_dump::read_dump(const proc_info& p_info) {
    proc_reader reader(p_info);

    m_dump = std::shared_ptr<std::uint8_t[]>(new std::uint8_t[m_length.value()]);
    std::size_t byte_to_read = reader.read_byte_sequence(m_address.value(), m_length.value(), m_dump.get());

    if (byte_to_read == 0) {
        LOG_ERROR_WITH_WAIT_KEY_AND_RETURN_VALUE("Impossible to read process memory at '" + std::to_string(m_address.value()) 
            + "' with size '" + std::to_string(m_length.value()) + "'.", false)
    }

    if (byte_to_read != m_length.value()) {
        LOG_WARNING_WITH_WAIT_KEY("Impossible to read requested process memory size '" + std::to_string(m_length.value()) 
            + "' at '" + std::to_string(m_address.value()) + "'. Only '" + std::to_string(byte_to_read) 
            + "' bytes were read from '" + std::to_string(m_length.value()) + "' requested bytes.")

        m_length = byte_to_read;
    }

    return true;
}


void state_dump::show_dump(const context& p_context) const {
    static const std::size_t row_size = 16;
    static const std::string string_separator = "    | ";

    console::clear();

    intro_builder::show(p_context, "View memory dump at the specific address.");

    std::cout << "Address: " << std::hex << m_address.value() << ", Length: " << std::dec << m_length.value() << std::endl << std::endl;

    std::size_t row_number = 0;
    std::string line;
    for (std::size_t i = 0; i < m_length.value(); i++) {
        if ((i % row_size) == 0) {
            line.clear();
            std::cout << '\t' << (void *) (m_address.value() + row_number * row_size) << " | ";
        }

        const int byte_value = static_cast<int>(m_dump.get()[i]);

        std::stringstream byte_stream;
        if (byte_value < 0x10) {
            byte_stream << "0";
        }

        byte_stream << std::uppercase << std::hex << byte_value;

        if ((byte_value >= 32) && (byte_value <= 126)) {
            line.push_back(m_dump.get()[i]);
        }
        else {
            line.push_back('.');
        }

        std::cout << byte_stream.str() << " ";

        if ((i + 1) % row_size == 0) {
            row_number++;
            std::cout << string_separator << line << std::endl;
        }
    }

    const std::size_t amount_bytes_last_line = m_length.value() % row_size;
    if (amount_bytes_last_line != 0) {
        const std::size_t amount_miss_bytes = row_size - amount_bytes_last_line;

        const std::string filler(amount_miss_bytes * 3, ' ');
        std::cout << filler << string_separator << line << std::endl;
    }

    std::cout << std::endl << std::endl;
}


std::ostream& operator<<(std::ostream& p_stream, const state_dump&) {
    p_stream << "state_dump";
    return p_stream;
}

