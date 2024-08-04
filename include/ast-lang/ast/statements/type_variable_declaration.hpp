#ifndef CCL_PROJECT_TYPE_VARIABLE_DECLARATION_HPP
#define CCL_PROJECT_TYPE_VARIABLE_DECLARATION_HPP

#include <ast-lang/ast/node.hpp>

namespace astlang::ast::statement
{
    class TypeVariableDeclaration : public Node
    {
    public:
        using Node::Node;

        auto compute(interpreter::Interpreter &interpreter)
            -> interpreter::EvaluationResult override;
    };
}// namespace astlang::ast::statement

#endif /* CCL_PROJECT_TYPE_VARIABLE_DECLARATION_HPP */
