#ifndef CCL_PROJECT_ASTLANG_AST_LANG_HPP
#define CCL_PROJECT_ASTLANG_AST_LANG_HPP

#include <ankerl/unordered_dense.h>
#include <ccl/ccl.hpp>
#include <ccl/parser/types.hpp>
#include <isl/string_view.hpp>

#define ASTLANG_DECL CCL_DECL
#define ASTLANG_INLINE CCL_INLINE
#define ASTLANG_LIFETIMEBOUND CCL_LIFETIMEBOUND
#define ASTLANG_TRIVIAL_ABI CCL_TRIVIAL_ABI
#define ASTLANG_NOEXCEPT_IF(EXPR) CCL_NOEXCEPT_IF(EXPR)

namespace astlang2
{
    enum class NodeTypes : ccl::SmallId
    {
        TRUE = 128,
        FALSE,
        NUMBER,
        FLOAT,
        STRING,
        IDENTIFIER,
        FUNCTION_CALL,
        METHOD_CALL,
        VALUE,
        FACTOR,
        TERM,
        EXPRESSION,
        COMPARISON,
        EQUALITY,
        BITWISE_AND_EXPRESSION,
        BITWISE_XOR_EXPRESSION,
        BITWISE_OR_EXPRESSION,
        LOGICAL_AND_EXPRESSION,
        LOGICAL_OR_EXPRESSION,
        ASSIGNMENT_EXPRESSION,
        PLUS,
        MINUS,
        MULTIPLY,
        DIVIDE,
        MODULUS,
        LESS,
        LESS_EQUAL,
        GREATER,
        GREATER_EQUAL,
        EQUAL,
        NOT_EQUAL,
        LOGICAL_AND,
        LOGICAL_OR,
        BITWISE_AND,
        BITWISE_OR,
        BITWISE_XOR,
        BITWISE_NOT,
        ASSIGN,
    };

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

    auto getAstlangGrammar() -> isl::string_view;
    auto getNodeTypesMap() -> const ankerl::unordered_dense::map<std::string, SmallId> &;
}// namespace astlang2

#endif /* CCL_PROJECT_ASTLANG_AST_LANG_HPP */
