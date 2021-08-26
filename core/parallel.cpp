#include "parallel.h"


namespace crwproc
{

namespace parallel
{


std::size_t get_amount_threads() {
    const static std::size_t amount_threads = (std::thread::hardware_concurrency() > 1) ? (std::thread::hardware_concurrency() - 1) : 0;
    return amount_threads;
}


}

}