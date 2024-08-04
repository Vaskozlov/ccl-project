#ifndef CCL_PROJECT_ASTLANG_LOGICAL_AND_HPP
#define CCL_PROJECT_ASTLANG_LOGICAL_AND_HPP

#include <ast-lang/ast/expression/binary_expression_base.hpp>

namespace astlang::ast::expression
{
    class LogicalAnd : public BinaryExpressionBase
    {
    public:
        using BinaryExpressionBase::BinaryExpressionBase;

        auto compute(Interpreter &interpreter) -> EvaluationResult override;
    };
}// namespace astlang::ast::expression

#endif /* CCL_PROJECT_ASTLANG_LOGICAL_AND_HPP */
