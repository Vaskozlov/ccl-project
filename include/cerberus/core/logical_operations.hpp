#ifndef CERBERUS_PROJECT_LOGICAL_OPERATIONS_HPP
#define CERBERUS_PROJECT_LOGICAL_OPERATIONS_HPP

#include <cerberus/core/concepts.hpp>
#include <cerberus/core/defines.hpp>
#include <utility>

namespace cerb
{
    template<Boolean... Ts>
    CERBLIB_DECL auto land(Ts &&...args) -> bool
    {
        return (... && std::forward<Ts>(args));
    }

    template<Boolean... Ts>
    CERBLIB_DECL auto lor(Ts &&...args) -> bool
    {
        return (... || std::forward<Ts>(args));
    }
}// namespace cerb

#endif /* CERBERUS_PROJECT_LOGICAL_OPERATIONS_HPP */
