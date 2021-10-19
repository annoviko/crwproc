/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#include "storage.h"

#include <cstring>


storage::storage() {
    std::memset(this, 0, sizeof(storage));
}
