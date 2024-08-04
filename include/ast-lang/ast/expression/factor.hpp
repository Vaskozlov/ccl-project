#ifndef CCL_PROJECT_ASTLANG_FACTOR_HPP
#define CCL_PROJECT_ASTLANG_FACTOR_HPP

#include <ast-lang/ast/node.hpp>
#include <ccl/text/remove_escaping.hpp>

namespace astlang::ast::expression
{
    class Factor : public Node
    {
    public:
        using Node::Node;

        auto compute(Interpreter &interpreter) -> EvaluationResult override;

    private:
        auto computeExpression(Interpreter &interpreter) -> EvaluationResult;
    };
}// namespace astlang::ast::expression

#endif /* CCL_PROJECT_ASTLANG_FACTOR_HPP */
