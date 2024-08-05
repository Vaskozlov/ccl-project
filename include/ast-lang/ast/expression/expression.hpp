#ifndef CCL_PROJECT_ASTLANG_EXPRESSION_HPP
#define CCL_PROJECT_ASTLANG_EXPRESSION_HPP

#include <ast-lang/ast/expression/binary_expression_base.hpp>

namespace astlang::ast::expression
{
    class Expression : public BinaryExpressionBase
    {
    public:
        using BinaryExpressionBase::BinaryExpressionBase;

        auto compute(Interpreter &interpreter) const -> EvaluationResult override;
    };
}// namespace astlang::ast::expression

#endif /* CCL_PROJECT_ASTLANG_EXPRESSION_HPP */
