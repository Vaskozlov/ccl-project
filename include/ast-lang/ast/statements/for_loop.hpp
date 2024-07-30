#ifndef CCL_PROJECT_FOR_LOOP_HPP
#define CCL_PROJECT_FOR_LOOP_HPP

#include <ast-lang/ast/node_sequence.hpp>

namespace astlang::ast::statement
{
    class ForLoop
      : public ccl::parser::ast::UnNodeSequence
      , public NodeSequence
    {
    public:
        using ccl::parser::ast::UnNodeSequence::NodeSequence;

        auto
            compute(interpreter::Interpreter &interpreter) -> interpreter::EvaluationResult override
        {
            return interpreter::EvaluationResult{
                .value = std::nullopt, .type = interpreter::Type::VOID};
        }
    };
}// namespace astlang::ast::statement

#endif /* CCL_PROJECT_FOR_LOOP_HPP */
