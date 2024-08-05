#ifndef CCL_PROJECT_ASTLANG_RETURN_STATEMENT_HPP
#define CCL_PROJECT_ASTLANG_RETURN_STATEMENT_HPP

#include <ast-lang/ast/node.hpp>

namespace astlang::ast::statement
{
    class ReturnStatement : public Node
    {
    public:
        using Node::Node;

        auto compute(Interpreter &interpreter) const -> EvaluationResult override;
    };
}// namespace astlang::ast::statement

#endif /* CCL_PROJECT_ASTLANG_RETURN_STATEMENT_HPP */
