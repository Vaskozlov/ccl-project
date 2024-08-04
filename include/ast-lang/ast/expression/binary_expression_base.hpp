#ifndef CCL_PROJECT_ASTLANG_BINARY_EXPRESSION_BASE_HPP
#define CCL_PROJECT_ASTLANG_BINARY_EXPRESSION_BASE_HPP

#include <ast-lang/ast/node.hpp>
#include <ast-lang/interpreter/function_interface.hpp>

namespace astlang::ast::expression
{
    class BinaryExpressionBase : public Node
    {
    public:
        using Node::Node;

        auto computeBinaryExpression(
            Interpreter &interpreter,
            const std::vector<std::pair<SmallId, std::string>> &token_to_function_name)
            -> interpreter::EvaluationResult;
    };
}// namespace astlang::ast::expression

#endif /* CCL_PROJECT_ASTLANG_BINARY_EXPRESSION_BASE_HPP */
