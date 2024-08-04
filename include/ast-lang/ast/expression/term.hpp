#ifndef CCL_PROJECT_TERM_HPP
#define CCL_PROJECT_TERM_HPP

#include <ast-lang/ast/node.hpp>

namespace astlang::ast::expression
{
    class Term : public Node
    {
    public:
        using Node::Node;

        auto compute(Interpreter &interpreter) -> EvaluationResult override;
    };
}// namespace astlang::ast::expression

#endif /* CCL_PROJECT_TERM_HPP */
