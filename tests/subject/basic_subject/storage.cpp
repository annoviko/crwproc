#include "storage.h"

#include <cstring>


storage::storage() {
    std::memset(this, 0, sizeof(storage));
}
