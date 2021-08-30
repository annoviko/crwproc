/*!

@authors Andrei Novikov (spb.andr@yandex.ru)
@copyright BSD-3-Clause

*/


#pragma once


#include <type_traits>


namespace crwproc {

namespace traits
{


template <typename ActualType, typename ... OtherTypes>
struct is_any : std::disjunction<std::is_same<ActualType, OtherTypes> ...> { };


}

}
