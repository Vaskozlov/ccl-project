#ifndef CCL_PROJECT_ASTLANG_METHOD_CALL_HPP
#define CCL_PROJECT_ASTLANG_METHOD_CALL_HPP

#include <ast-lang/ast/node.hpp>
#include <ast-lang/interpreter/evaluation_result.hpp>

namespace astlang::ast::function::call
{
    class MethodCall : public Node
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
}// namespace astlang::ast::function::call

#endif /* CCL_PROJECT_ASTLANG_METHOD_CALL_HPP */
