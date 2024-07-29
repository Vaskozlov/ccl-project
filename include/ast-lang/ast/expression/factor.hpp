#ifndef CCL_PROJECT_FACTOR_HPP
#define CCL_PROJECT_FACTOR_HPP

#include <ast-lang/ast/node_sequence.hpp>
#include <ccl/text/remove_escaping.hpp>
#include <charconv>

namespace astlang::ast::expression
{
    class Factor
      : public ccl::parser::ast::UnNodeSequence
      , public NodeSequence
    {
    public:
        using ccl::parser::ast::UnNodeSequence::NodeSequence;

        auto compute(interpreter::Interpreter &interpreter)
            -> interpreter::EvaluationResult override;

    private:
        auto computeExpression(interpreter::Interpreter &interpreter)
            -> interpreter::EvaluationResult;
    };
}// namespace astlang::ast::expression

#endif /* CCL_PROJECT_FACTOR_HPP */
