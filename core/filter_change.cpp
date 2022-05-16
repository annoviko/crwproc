/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "filter_change.h"


filter_change::filter_change(const type_desc& p_type) :
    filter_base(p_type)
{ }


std::ostream& operator<<(std::ostream& p_stream, const filter_change&) {
    p_stream << "FilterChange (x[t] != x[t-1])";
    return p_stream;
}
