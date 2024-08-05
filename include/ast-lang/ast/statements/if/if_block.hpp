#ifndef CCL_PROJECT_ASTLANG_IF_BLOCK_HPP
#define CCL_PROJECT_ASTLANG_IF_BLOCK_HPP

#include <ast-lang/ast/node.hpp>

namespace astlang::ast::statement::if_statement
{
    class IfBlock : public Node
    {
    public:
        using Node::Node;

        auto compute(Interpreter &interpreter) const -> EvaluationResult override;
    };
}// namespace astlang::ast::statement::if_statement

#endif /* CCL_PROJECT_ASTLANG_IF_BLOCK_HPP */
