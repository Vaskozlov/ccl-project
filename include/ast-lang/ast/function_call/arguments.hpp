#ifndef CCL_PROJECT_ARGUMENTS_HPP
#define CCL_PROJECT_ARGUMENTS_HPP

#include <ast-lang/ast/node_sequence.hpp>

namespace astlang::ast::function::call
{
    class Arguments
      : public ccl::parser::ast::UnNodeSequence
      , public NodeSequence
    {
    public:
        using ccl::parser::ast::UnNodeSequence::NodeSequence;

        auto compute(interpreter::Interpreter &interpreter)
            -> interpreter::EvaluationResult override;
    };
}// namespace astlang::ast::function::call

#endif /* CCL_PROJECT_ARGUMENTS_HPP */
