/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once


namespace crwproc {

namespace console {


struct position {
public:
    int x = 0;
    int y = 0;

public:
    bool operator==(const position& p_other) const;

    bool operator<(const position& p_other) const;

    bool operator>(const position& p_other) const;
};


}

}
