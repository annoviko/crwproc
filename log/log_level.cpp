/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "log_level.h"

#include <string>


std::ostream& operator<<(std::ostream& p_stream, const log_level& p_level) {
    const static std::string dictionary[] = { "[TRACE]", "[DEBUG]", "[INFO]", "[WARNING]", "[ERROR]", "[FATAL]" };

    const std::size_t index = static_cast<std::size_t>(p_level);
    p_stream << ((index < std::size(dictionary)) ? dictionary[index] : "[UNKNOWN]");

    return p_stream;
}
