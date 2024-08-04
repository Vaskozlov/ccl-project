#ifndef CCL_PROJECT_FUNCTION_DEFINITION_ARGUMENTS_HPP
#define CCL_PROJECT_FUNCTION_DEFINITION_ARGUMENTS_HPP

#include <ast-lang/ast/node.hpp>

namespace astlang::ast::function::decl
{
    class Arguments : public Node
    {
    public:
        using Node::Node;

        auto compute(Interpreter &interpreter) -> EvaluationResult override;
    };
}// namespace astlang::ast::function::decl

#endif /* CCL_PROJECT_FUNCTION_DEFINITION_ARGUMENTS_HPP */
