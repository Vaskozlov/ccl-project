#ifndef AST_LANG_2_BINARY_EXPRESSION_HPP
#define AST_LANG_2_BINARY_EXPRESSION_HPP

#include <ast-lang-2/ast/core/basic_value.hpp>

namespace astlang2::ast::expression
{
    class BinaryExpression final : public core::BasicValue
    {
    public:
        using BasicValue::BasicValue;

        auto compute(interpreter::Interpreter &interpreter) const
            -> core::ComputationResult override;
    };
}// namespace astlang2::ast::expression

#endif /* AST_LANG_2_BINARY_EXPRESSION_HPP */
