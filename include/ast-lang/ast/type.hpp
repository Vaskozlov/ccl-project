#ifndef CCL_PROJECT_ASTLANG_TYPE_HPP
#define CCL_PROJECT_ASTLANG_TYPE_HPP

#include <ast-lang/ast/node.hpp>

namespace astlang::ast
{
    class Type final : public Node
    {
    public:
        using Node::Node;

        auto compute(Interpreter &interpreter) const -> EvaluationResult override
        {
            return interpreter::EvaluationResult{
                // .instance = Instance{.type = interpreter.typeSystem.getType("void")},
                // .needToReturn = false,
                // .storesReference = false,
            };
        }
    };
}// namespace astlang::ast

#endif /* CCL_PROJECT_ASTLANG_TYPE_HPP */
