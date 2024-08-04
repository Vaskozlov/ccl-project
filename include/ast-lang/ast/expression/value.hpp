#ifndef CCL_PROJECT_ASTLANG_VALUE_HPP
#define CCL_PROJECT_ASTLANG_VALUE_HPP

#include <ast-lang/ast/node.hpp>

namespace astlang::ast::expression
{
    class Value : public Node
    {
    public:
        using Node::Node;

        auto compute(Interpreter &interpreter) -> EvaluationResult override;
    };
}// namespace astlang::ast::expression

#endif /* CCL_PROJECT_ASTLANG_VALUE_HPP */
