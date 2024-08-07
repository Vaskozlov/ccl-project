#ifndef CCL_PROJECT_ASTLANG_ELSE_BLOCK_HPP
#define CCL_PROJECT_ASTLANG_ELSE_BLOCK_HPP

#include <ast-lang/ast/node.hpp>

namespace astlang::ast::statement::if_statement
{
    class ElseBlock : public Node
    {
    public:
        using Node::Node;

        auto compute(Interpreter &interpreter) const -> EvaluationResult override;
    };
}// namespace astlang::ast::statement::if_statement

#endif /* CCL_PROJECT_ASTLANG_ELSE_BLOCK_HPP */
