#ifndef CCL_PROJECT_ASTLANG_FUNCTION_DEFINITION_HPP
#define CCL_PROJECT_ASTLANG_FUNCTION_DEFINITION_HPP

#include <ast-lang/ast/node.hpp>

namespace astlang::ast::function::decl
{
    class Definition : public Node
    {
    public:
        using Node::Node;

        auto compute(Interpreter &interpreter) -> EvaluationResult override;
    };
}// namespace astlang::ast::function::decl


#endif /* CCL_PROJECT_ASTLANG_FUNCTION_DEFINITION_HPP */
