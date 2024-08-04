#ifndef CCL_PROJECT_ASTLANG_GLOBAL_DECLARATIONS_HPP
#define CCL_PROJECT_ASTLANG_GLOBAL_DECLARATIONS_HPP

#include <ast-lang/ast/node.hpp>

namespace astlang::ast
{
    class GlobalDeclarations : public Node
    {
    public:
        using Node::Node;

        auto compute(Interpreter &interpreter) -> EvaluationResult override;
    };
}// namespace astlang::ast

#endif /* CCL_PROJECT_ASTLANG_GLOBAL_DECLARATIONS_HPP */
