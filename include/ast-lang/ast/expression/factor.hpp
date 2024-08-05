#ifndef CCL_PROJECT_ASTLANG_FACTOR_HPP
#define CCL_PROJECT_ASTLANG_FACTOR_HPP

#include <ast-lang/ast/expression/basic_value.hpp>
#include <ccl/text/remove_escaping.hpp>

namespace astlang::ast::expression
{
    class Factor : public BasicValue
    {
    public:
        using BasicValue::BasicValue;

        auto compute(Interpreter &interpreter) const -> EvaluationResult override;

    private:
        auto computeExpression(Interpreter &interpreter) const -> EvaluationResult;
    };
}// namespace astlang::ast::expression

#endif /* CCL_PROJECT_ASTLANG_FACTOR_HPP */
