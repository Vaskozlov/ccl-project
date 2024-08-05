#ifndef CCL_PROJECT_ASTLANG_TYPE_HPP
#define CCL_PROJECT_ASTLANG_TYPE_HPP

#include <ast-lang/ast/node.hpp>

namespace astlang::ast
{
    class Type : public Node
    {
    public:
        using Node::Node;

        auto compute(Interpreter &interpreter) const -> EvaluationResult override
        {
            return interpreter::EvaluationResult{
                .value = std::nullopt, .type = interpreter::Type::VOID};
        }
    };
}// namespace astlang::ast

#endif /* CCL_PROJECT_ASTLANG_TYPE_HPP */
