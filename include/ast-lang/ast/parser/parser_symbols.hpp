#ifndef CCL_PROJECT_ASTLANG_PARSER_SYMBOLS_HPP
#define CCL_PROJECT_ASTLANG_PARSER_SYMBOLS_HPP

#include <ast-lang/ast/node.hpp>

namespace astlang::ast
{
    class ParserSymbols : public Node
    {
    public:
        using Node::Node;

        auto compute(Interpreter &interpreter) -> EvaluationResult override
        {
            return interpreter::EvaluationResult{
                .value = std::nullopt, .type = interpreter::Type::VOID};
        }
    };
}// namespace astlang::ast

#endif /* CCL_PROJECT_ASTLANG_PARSER_SYMBOLS_HPP */
