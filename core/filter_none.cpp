/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/

#include "filter_none.h"


bool filter_none::is_valid() const { return true; }


bool filter_none::is_value_signed() const { throw std::logic_error("Filter None does not have value."); }


value::type filter_none::get_value_type() const { throw std::logic_error("Filter None does not have value."); }


std::size_t filter_none::get_value_size() const { throw std::logic_error("Filter None does not have value."); }


std::ostream& operator<<(std::ostream& p_stream, const filter_none& p_info) {
    p_stream << "No Filter";
    return p_stream;
}