/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "filter_base.h"


filter_base::filter_base(const type_desc& p_type) :
    m_type(p_type)
{ }


bool filter_base::is_valid() const {
    return m_type.is_valid();
}


const type_desc& filter_base::get_type() const {
    return m_type;
}


type_desc& filter_base::get_type() {
    return m_type;
}