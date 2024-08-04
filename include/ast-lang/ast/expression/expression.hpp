#ifndef CCL_PROJECT_EXPRESSION_HPP
#define CCL_PROJECT_EXPRESSION_HPP

#include <ast-lang/ast/node.hpp>

namespace astlang::ast::expression
{
    class Expression : public Node
    {
    public:
        using Node::Node;

        auto compute(interpreter::Interpreter &interpreter)
            -> interpreter::EvaluationResult override;
    };
}// namespace astlang::ast::expression

#endif /* CCL_PROJECT_EXPRESSION_HPP */
