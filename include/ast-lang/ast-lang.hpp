#ifndef CCL_PROJECT_ASTLANG_AST_LANG_HPP
#define CCL_PROJECT_ASTLANG_AST_LANG_HPP

#include <ankerl/unordered_dense.h>
#include <ccl/ccl.hpp>
#include <ccl/parser/types.hpp>

#define ASTLANG_DECL CCL_DECL
#define ASTLANG_INLINE CCL_INLINE
#define ASTLANG_LIFETIMEBOUND CCL_LIFETIMEBOUND
#define ASTLANG_TRIVIAL_ABI CCL_TRIVIAL_ABI
#define ASTLANG_NOEXCEPT_IF(EXPR) CCL_NOEXCEPT_IF(EXPR)

namespace astlang
{
    using i8 = ccl::i8;
    using i16 = ccl::i16;
    using i32 = ccl::i32;
    using i64 = ccl::i64;

    using u8 = ccl::u8;
    using u16 = ccl::u16;
    using u32 = ccl::u32;
    using u64 = ccl::u64;

    using SmallId = ccl::SmallId;
    using State = ccl::parser::State;
    using Symbol = ccl::parser::Symbol;

    using f32 = ccl::f32;
    using f64 = ccl::f64;
}// namespace astlang

#endif /* CCL_PROJECT_ASTLANG_AST_LANG_HPP */
