#ifndef CCL_PROJECT_STATEMENT_HPP
#define CCL_PROJECT_STATEMENT_HPP

#include <ast-lang/ast/node_sequence.hpp>

namespace astlang::ast::statement
{
    class Statement
      : public ccl::parser::ast::UnNodeSequence
      , public NodeSequence
    {
    public:
        using ccl::parser::ast::UnNodeSequence::NodeSequence;

        auto
            compute(interpreter::Interpreter &interpreter) -> interpreter::EvaluationResult override
        {
            return NodePtr(this->front()).astlangNode->compute(interpreter);
        }
    };
}// namespace astlang::ast::statement

#endif /* CCL_PROJECT_STATEMENT_HPP */
