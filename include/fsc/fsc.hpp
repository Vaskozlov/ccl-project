#ifndef CCL_PROJECT_FSC_HPP
#define CCL_PROJECT_FSC_HPP

#include <ccl/ccl.hpp>
#include <ccl/lex/token.hpp>

namespace fsc
{
    using u8 = ccl::u8;
    using u16 = ccl::u16;
    using u32 = ccl::u32;
    using u64 = ccl::u64;

    using i8 = ccl::i8;
    using i16 = ccl::i16;
    using i32 = ccl::i32;
    using i64 = ccl::i64;

    using Token = ccl::lex::Token;

    using ccl::makeUnique;// NOLINT
    using ccl::UniquePtr; // NOLINT
    using ccl::Vector;
}// namespace fsc

#endif /* CCL_PROJECT_FSC_HPP */
