#ifndef CCL_PROJECT_ASTLANG_GOAL_HPP
#define CCL_PROJECT_ASTLANG_GOAL_HPP

#include <ast-lang/ast/node.hpp>

namespace astlang::ast
{
    class Goal final : public Node
    {
    public:
        using Node::Node;

        auto compute(Interpreter &interpreter) const -> EvaluationResult override
        {
            return interpreter::EvaluationResult{
                .instance = Instance{},
                .needToReturn = false,
                .storesReference = false,
            };
        }
    };
}// namespace astlang::ast

#endif /* CCL_PROJECT_ASTLANG_GOAL_HPP */
