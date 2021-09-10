/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/

#include "filter_none.h"


bool filter_none::is_valid() const { return true; };


const type_desc& filter_none::get_type() const { throw std::logic_error("Filter None does not have type."); };


type_desc& filter_none::get_type() { throw std::logic_error("Filter None does not have type."); };


std::ostream& operator<<(std::ostream& p_stream, const filter_none& p_filter) {
    (void)p_filter;

    p_stream << "No Filter";
    return p_stream;
}