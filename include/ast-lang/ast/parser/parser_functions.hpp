#ifndef CCL_PROJECT_ASTLANG_PARSER_FUNCTIONS_HPP
#define CCL_PROJECT_ASTLANG_PARSER_FUNCTIONS_HPP

#include <ast-lang/ast/node.hpp>

namespace astlang::ast
{
    class ParserFunctions : public Node
    {
    public:
        using Node::Node;

        auto compute(Interpreter &interpreter) const -> EvaluationResult override
        {
            return interpreter::EvaluationResult{};
        }
    };
}// namespace astlang::ast


#endif /* CCL_PROJECT_ASTLANG_PARSER_FUNCTIONS_HPP */
