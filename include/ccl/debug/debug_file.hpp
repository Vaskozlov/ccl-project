#ifndef CCL_PROJECT_DEBUG_FILE_HPP
#define CCL_PROJECT_DEBUG_FILE_HPP

#include <catch2/catch_test_macros.hpp>
#include <ccl/ccl.hpp>
#include <ccl/lexer/token.hpp>

#define DEBUG_VAR [[maybe_unused]] auto &&

namespace ccl::debug
{
    constexpr inline size_t FirstUsableToken = lexer::ReservedTokenMaxValue + 1;
}

#endif /* CCL_PROJECT_DEBUG_FILE_HPP */
