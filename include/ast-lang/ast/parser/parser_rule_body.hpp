#ifndef CCL_PROJECT_PARSER_RULE_BODY_HPP
#define CCL_PROJECT_PARSER_RULE_BODY_HPP

#include <ast-lang/ast/node_sequence.hpp>

namespace astlang::ast
{
    class ParserRuleBody
      : public ccl::parser::ast::UnNodeSequence
      , public NodeSequence
    {
    public:
        using ccl::parser::ast::UnNodeSequence::NodeSequence;

        auto
            compute(interpreter::Interpreter &interpreter) -> interpreter::EvaluationResult override
        {}
    };
}// namespace astlang::ast


#endif /* CCL_PROJECT_PARSER_RULE_BODY_HPP */
