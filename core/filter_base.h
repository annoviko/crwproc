/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/

#pragma once


#include "type_desc.h"


class filter_base {
protected:
    type_desc m_type;

public:
    filter_base() = default;

    filter_base(const type_desc& p_type);

public:
    virtual bool is_valid() const;

    virtual const type_desc& get_type() const;

    virtual type_desc& get_type();
};