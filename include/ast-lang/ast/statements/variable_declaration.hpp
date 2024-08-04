#ifndef CCL_PROJECT_ASTLANG_VARIABLE_DECLARATION_HPP
#define CCL_PROJECT_ASTLANG_VARIABLE_DECLARATION_HPP

#include <ast-lang/ast/node.hpp>

namespace astlang::ast::statement
{
    class VariableDeclaration : public Node
    {
    public:
        using Node::Node;

        auto compute(Interpreter &interpreter) -> EvaluationResult override;
    };
}// namespace astlang::ast::statement

#endif /* CCL_PROJECT_ASTLANG_VARIABLE_DECLARATION_HPP */
