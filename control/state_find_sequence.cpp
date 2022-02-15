/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "state_find_sequence.h"

#include <set>

#include "asker.h"
#include "intro_builder.h"

#include "core/proc_reader.h"
#include "core/string_numeric.h"


event state_find_sequence::operator()(const context& p_context) {
    intro_builder::show(p_context, "Find byte sequence in the process memory.");

    if (!ask_byte_sequence()) {
        return event_error{ };
    }

    find_byte_sequence(p_context);

    return continue_until_event_is_not<event_refresh>([this, &p_context]() {
        show_result(p_context);
        return ask_next_action(p_context);
    });
}


bool state_find_sequence::ask_byte_sequence() {
    std::cout << "Enter byte sequence to find: ";

    std::cin.clear();
    std::cin.ignore(1024, '\n');

    std::string byte_sequence;
    std::getline(std::cin, byte_sequence);

    const std::set<char> symbols_to_ignore = { ' ', '\n', '\r' };

    std::string byte_string_value;
    for (const char symbol : byte_sequence) {
        if (symbols_to_ignore.find(symbol) != symbols_to_ignore.cend()) {
            continue;
        }

        const char symbol_to_add = static_cast<char>(std::toupper(symbol));
        byte_string_value.push_back(symbol_to_add);

        if (byte_string_value.size() == 2) {
            if (!add_byte(byte_string_value)) {
                return false;
            }
        }
    }

    if (!byte_string_value.empty()) {
        if (!add_byte(byte_string_value)) {
            return false;
        }
    }

    return true;
}


bool state_find_sequence::add_byte(std::string& p_byte) {
    const auto byte_value = crwproc::string::stonum<std::uint8_t>(p_byte, 16);
    if (!byte_value.has_value()) {
        return false;
    }

    m_sequence.push_back(byte_value.value());
    p_byte.clear();

    return true;
}


void state_find_sequence::find_byte_sequence(const context& p_context) {
    proc_reader reader(p_context.get_proc_info());
    m_address = reader.find_byte_sequence(m_sequence, m_sequence.size());
}


void state_find_sequence::show_result(const context& p_context) const {
    crwproc::console::control::clear();

    intro_builder::show(p_context, "Find byte sequence in the process memory.");

    std::cout << std::endl << "Byte sequence: ";
    for (const std::uint8_t byte : m_sequence) {
        if (byte < 10) {
            std::cout << '0';
        }

        std::cout << std::to_string(byte) << ' ';
    }

    std::cout << std::endl << std::endl;

    if (m_address != 0) {
        std::cout << "Byte sequence has been found at '" << (void *) m_address << "'." << std::endl;
    }
    else {
        crwproc::console::control::warning("Byte sequence has not been found.");
    }

    std::cout << std::endl << std::endl;
}


std::ostream& operator<<(std::ostream& p_stream, const state_find_sequence&) {
    p_stream << "state_find_sequence";
    return p_stream;
}
