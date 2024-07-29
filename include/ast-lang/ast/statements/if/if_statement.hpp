#ifndef CCL_PROJECT_IF_HPP
#define CCL_PROJECT_IF_HPP

#include <ast-lang/ast/node_sequence.hpp>

namespace astlang::ast::statement::if_statement
{
    class IfStatement
      : public ccl::parser::ast::UnNodeSequence
      , public NodeSequence
    {
    public:
        using ccl::parser::ast::UnNodeSequence::NodeSequence;

        auto
            compute(interpreter::Interpreter &interpreter) -> interpreter::EvaluationResult override
        {}
    };
}// namespace astlang::ast::statement::if_statement

#endif /* CCL_PROJECT_IF_HPP */
