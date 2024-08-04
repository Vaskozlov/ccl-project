#ifndef CCL_PROJECT_ASTLANG_METHOD_CALL_HPP
#define CCL_PROJECT_ASTLANG_METHOD_CALL_HPP

#include <ast-lang/ast/node.hpp>
#include <ast-lang/interpreter/evaluation_result.hpp>
#include <ast-lang/interpreter/type.hpp>

namespace astlang::ast::function::call
{
    class MethodCall : public Node
    {
    public:
        using Node::Node;

        auto compute(Interpreter &interpreter) -> EvaluationResult override
        {
            return interpreter::EvaluationResult{
                .value = std::nullopt, .type = interpreter::Type::VOID};
        }
    };
}// namespace astlang::ast::function::call

#endif /* CCL_PROJECT_ASTLANG_METHOD_CALL_HPP */
