/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "filter_less.h"


filter_less::filter_less(const type_desc& p_type) :
    filter_base(p_type)
{ }


std::ostream& operator<<(std::ostream& p_stream, const filter_less&) {
    p_stream << "FilterLess (x[t] < x[t-1])";
    return p_stream;
}
