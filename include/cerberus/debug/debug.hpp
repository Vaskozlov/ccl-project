#ifndef CERBERUS_PROJECT_DEBUG_HPP
#define CERBERUS_PROJECT_DEBUG_HPP

#include <cerberus/comparison.hpp>
#include <cerberus/core/defines.hpp>

#define STATIC_VARIABLE [[maybe_unused]] static constexpr
#define GLOBAL_SCOPE [[maybe_unused]] static auto CERBLIB_UNIQUE_IDENT()->void
#define STATIC_ASSERT_EQ(lhs, rhs) static_assert(cerb::safeEqual(lhs, rhs))

#endif /* CERBERUS_PROJECT_DEBUG_HPP */
