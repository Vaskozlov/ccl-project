#ifndef CCL_PROJECT_ASTLANG_STATEMENT_HPP
#define CCL_PROJECT_ASTLANG_STATEMENT_HPP

#include <ast-lang/ast/node.hpp>

namespace astlang::ast::statement
{
    class Statement : public Node
    {
    public:
        using Node::Node;

        auto compute(Interpreter &interpreter) -> EvaluationResult override
        {
            return NodePtr(this->front()).astlangNode->compute(interpreter);
        }
    };
}// namespace astlang::ast::statement

#endif /* CCL_PROJECT_ASTLANG_STATEMENT_HPP */
