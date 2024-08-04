#ifndef CCL_PROJECT_ASTLANG_FOR_LOOP_HPP
#define CCL_PROJECT_ASTLANG_FOR_LOOP_HPP

#include <ast-lang/ast/node.hpp>

namespace astlang::ast::statement
{
    class ForLoop : public Node
    {
    public:
        using Node::Node;

        auto compute(Interpreter &interpreter) -> EvaluationResult override
        {
            return EvaluationResult{.value = std::nullopt, .type = interpreter::Type::VOID};
        }
    };
}// namespace astlang::ast::statement

#endif /* CCL_PROJECT_ASTLANG_FOR_LOOP_HPP */
