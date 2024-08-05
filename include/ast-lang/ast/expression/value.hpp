#ifndef CCL_PROJECT_ASTLANG_VALUE_HPP
#define CCL_PROJECT_ASTLANG_VALUE_HPP

#include <ast-lang/ast/expression/basic_value.hpp>

namespace astlang::ast::expression
{
    class Value : public BasicValue
    {
    public:
        using BasicValue::BasicValue;

        auto compute(Interpreter &interpreter) const -> EvaluationResult override;
    };
}// namespace astlang::ast::expression

#endif /* CCL_PROJECT_ASTLANG_VALUE_HPP */
