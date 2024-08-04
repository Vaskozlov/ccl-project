#ifndef CCL_PROJECT_GLOBAL_DECLARATION_HPP
#define CCL_PROJECT_GLOBAL_DECLARATION_HPP

#include <ast-lang/ast/node.hpp>

namespace astlang::ast
{
    class GlobalDeclaration : public Node
    {
    public:
        using Node::Node;

        auto compute(Interpreter &interpreter) -> EvaluationResult override;
    };
}// namespace astlang::ast

#endif /* CCL_PROJECT_GLOBAL_DECLARATION_HPP */
