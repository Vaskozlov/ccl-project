#ifndef CCL_PROJECT_ARGUMENT_HPP
#define CCL_PROJECT_ARGUMENT_HPP

#include <ast-lang/ast/node.hpp>

namespace astlang::ast::function::decl
{
    class Argument : public Node
    {
    public:
        using Node::Node;

        auto compute(interpreter::Interpreter &interpreter)
            -> interpreter::EvaluationResult override;
    };
}// namespace astlang::ast::function::decl

#endif /* CCL_PROJECT_ARGUMENT_HPP */
