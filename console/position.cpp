/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "position.h"


namespace crwproc {

namespace console {


bool position::operator==(const position& p_other) const {
    return (x == p_other.x) && (y == p_other.y);
}


bool position::operator<(const position& p_other) const {
    if (y < p_other.y) {
        return true;
    }

    if ((y == p_other.y) && (x < p_other.x)) {
        return true;
    }

    return false;
}


bool position::operator>(const position& p_other) const {
    if (y > p_other.y) {
        return true;
    }

    if ((y == p_other.y) && (x > p_other.x)) {
        return true;
    }

    return false;
}


}

}