/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "intro_builder.h"

#include "log/logging.h"

#include <exception>
#include <memory>

#include <windows.h>


intro_builder::intro_builder(const std::string& p_intro) :
    m_intro(p_intro)
{ }


void intro_builder::show(const context& p_context) {
    m_begin_position = console::get_cursor_position();
    show(p_context, m_intro);
    m_end_position = console::get_cursor_position();
}


void intro_builder::redraw(const context& p_context) const {
    position original_position = console::get_cursor_position();

    const int lines_to_clean = m_end_position.y - m_begin_position.y - 1;
    if (lines_to_clean < 0) {
        /* it is possible in case of redefined output stream (sct-testing as an example). */
        LOG_WARNING("Impossible to redraw intro (probably output stream is redefined). Just draw intro.")
        show(p_context, m_intro);
    }

    console::set_cursor_position(m_begin_position);

    console::clear_lines(lines_to_clean);
    show(p_context, m_intro);

    console::set_cursor_position(original_position);
}


void intro_builder::show(const context& p_context, const std::string& p_intro) {
    static const std::string line(60, '-');

    std::cout << line << std::endl;
    std::cout << p_context;
    std::cout << line << std::endl;

    std::cout << line << std::endl;
    std::cout << p_intro << std::endl;
    std::cout << line << std::endl;
}
