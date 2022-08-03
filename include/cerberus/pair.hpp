#ifndef CERBERUS_PROJECT_PAIR_HPP
#define CERBERUS_PROJECT_PAIR_HPP

#include <cerberus/cerberus.hpp>

namespace cerb
{
    template<typename T1, typename T2>
    struct CERBLIB_TRIVIAL_ABI Pair
    {
        T1 first;
        T2 second;
    };
}// namespace cerb

#endif /* CERBERUS_PROJECT_PAIR_HPP */
