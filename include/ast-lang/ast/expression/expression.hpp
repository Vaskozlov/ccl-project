#ifndef CCL_PROJECT_EXPRESSION_HPP
#define CCL_PROJECT_EXPRESSION_HPP

#include <ast-lang/ast/node_sequence.hpp>

namespace astlang::ast::expression
{
    class Expression
      : public ccl::parser::ast::UnNodeSequence
      , public NodeSequence
    {
    public:
        using ccl::parser::ast::UnNodeSequence::NodeSequence;

        auto compute(interpreter::Interpreter &interpreter)
            -> interpreter::EvaluationResult override;
    };
}// namespace astlang::ast::expression

#endif /* CCL_PROJECT_EXPRESSION_HPP */
