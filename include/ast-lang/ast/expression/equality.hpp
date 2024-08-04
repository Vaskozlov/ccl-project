#ifndef CCL_PROJECT_ASTLANG_EQUALITY_HPP
#define CCL_PROJECT_ASTLANG_EQUALITY_HPP

#include <ast-lang/ast/expression/binary_expression_base.hpp>

namespace astlang::ast::expression
{
    class Equality : public BinaryExpressionBase
    {
    public:
        using BinaryExpressionBase::BinaryExpressionBase;

        auto compute(Interpreter &interpreter) -> EvaluationResult override;
    };
}// namespace astlang::ast::expression

#endif /* CCL_PROJECT_ASTLANG_EQUALITY_HPP */
