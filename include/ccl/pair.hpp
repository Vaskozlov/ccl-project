#ifndef CCL_PROJECT_PAIR_HPP
#define CCL_PROJECT_PAIR_HPP

#include <ccl/ccl.hpp>

namespace ccl
{
    template<typename T1, typename T2>
    struct CCL_TRIVIAL_ABI Pair
    {
        T1 first;
        T2 second;
    };
}// namespace ccl

#endif /* CCL_PROJECT_PAIR_HPP */
