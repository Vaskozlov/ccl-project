#ifndef CCL_PROJECT_STATEMENTS_HPP
#define CCL_PROJECT_STATEMENTS_HPP

#include <ast-lang/ast/node.hpp>

namespace astlang::ast::statement
{
    class Statements : public Node
    {
    public:
        using Node::Node;

        auto compute(Interpreter &interpreter) -> EvaluationResult override;
    };
}// namespace astlang::ast::statement

#endif /* CCL_PROJECT_STATEMENTS_HPP */
