/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once


#include <cstddef>
#include <exception>
#include <future>
#include <string>
#include <thread>


namespace crwproc
{

namespace parallel
{


std::size_t get_amount_threads();


template <typename TypeIndex, typename TypeTaskWithTidx>
void parallel_for_with_tidx(TypeIndex p_begin, TypeIndex p_end, const TypeTaskWithTidx& p_task) {
    if (p_end < p_begin) {
        throw std::invalid_argument("Start index '" + std::to_string(p_begin) + "' must be less or equal than end index '" + std::to_string(p_end) + "' of the parallel loop.");
    }
    else if (p_end == p_begin) {
        return;
    }

    const TypeIndex interval_total_length = p_end - p_begin;
    const TypeIndex interval_partial_length = interval_total_length / TypeIndex(get_amount_threads());

    TypeIndex current_start = p_begin;
    TypeIndex current_end = p_begin + interval_partial_length;

    std::vector<std::future<void>> future_storage;
    future_storage.reserve(get_amount_threads());

    for (std::size_t tidx = 0; (tidx < get_amount_threads() - 1) && (current_end < p_end); ++tidx) {
        const auto async_task = [&p_task, tidx, current_start, current_end]() {
            for (TypeIndex i = current_start; i < current_end; ++i) {
                p_task(i, tidx);
            }
        };

        future_storage.push_back(std::async(std::launch::async, async_task));

        current_start = current_end;
        current_end += interval_partial_length;
    }

    const std::size_t current_tidx = get_amount_threads() - 1;
    for (TypeIndex i = current_start; i < p_end; ++i) {
        p_task(i, current_tidx);
    }

    for (auto& feature : future_storage) {
        feature.get();
    }
}


}

}