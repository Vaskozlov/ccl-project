#ifndef CCL_PROJECT_ASTLANG_ARRAY_HPP
#define CCL_PROJECT_ASTLANG_ARRAY_HPP

#include <ast-lang/ast/node.hpp>

namespace astlang::ast::expression
{
    class Array : public Node
    {
    public:
        using Node::Node;

        auto compute(Interpreter &interpreter) const -> EvaluationResult override;
    };
}// namespace astlang::ast::expression

#endif /* CCL_PROJECT_ASTLANG_ARRAY_HPP */
