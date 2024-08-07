#ifndef CCL_PROJECT_ASTLANG_TERM_HPP
#define CCL_PROJECT_ASTLANG_TERM_HPP

#include <ast-lang/ast/expression/binary_expression_base.hpp>

namespace astlang::ast::expression
{
    class Term : public BinaryExpressionBase
    {
    public:
        using BinaryExpressionBase::BinaryExpressionBase;

        auto compute(Interpreter &interpreter) const -> EvaluationResult override;
    };
}// namespace astlang::ast::expression

#endif /* CCL_PROJECT_ASTLANG_TERM_HPP */
