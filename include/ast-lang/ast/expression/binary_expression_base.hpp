#ifndef CCL_PROJECT_ASTLANG_BINARY_EXPRESSION_BASE_HPP
#define CCL_PROJECT_ASTLANG_BINARY_EXPRESSION_BASE_HPP

#include <ast-lang/ast/expression/basic_value.hpp>
#include <ast-lang/interpreter/function_interface.hpp>

namespace astlang::ast::expression
{
    class BinaryExpressionBase : public BasicValue
    {
    public:
        using BasicValue::BasicValue;

        auto computeBinaryExpression(
            Interpreter &interpreter,
            const std::vector<std::pair<SmallId, std::string>> &token_to_function_name) const
            -> interpreter::EvaluationResult;
    };
}// namespace astlang::ast::expression

#endif /* CCL_PROJECT_ASTLANG_BINARY_EXPRESSION_BASE_HPP */
