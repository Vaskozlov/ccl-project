#ifndef CCL_PROJECT_ASTLANG_PARSER_RULE_HPP
#define CCL_PROJECT_ASTLANG_PARSER_RULE_HPP

#include <ast-lang/ast/node.hpp>

namespace astlang::ast
{
    class ParserRule : public Node
    {
    public:
        using Node::Node;

        auto compute(Interpreter &interpreter) const -> EvaluationResult override
        {
            return EvaluationResult{.value = std::nullopt, .type = interpreter::Type::VOID};
        }
    };
}// namespace astlang::ast

#endif /* CCL_PROJECT_ASTLANG_PARSER_RULE_HPP */
