/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "filter_more.h"


filter_more::filter_more(const type_desc& p_type) : 
    filter_base(p_type)
{ }


std::ostream& operator<<(std::ostream& p_stream, const filter_more&) {
    p_stream << "FilterMore (x[t] > x[t-1])";
    return p_stream;
}
