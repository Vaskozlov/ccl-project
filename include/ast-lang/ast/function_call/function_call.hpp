#ifndef CCL_PROJECT_FUNCTION_CALL_HPP
#define CCL_PROJECT_FUNCTION_CALL_HPP

#include <ast-lang/ast/node_sequence.hpp>

namespace astlang::ast::function::call
{
    class FunctionCall
      : public ccl::parser::ast::UnNodeSequence
      , public NodeSequence
    {
    public:
        using ccl::parser::ast::UnNodeSequence::NodeSequence;

        auto compute(interpreter::Interpreter &interpreter)
            -> interpreter::EvaluationResult override;
    };
}// namespace astlang::ast::function::call

#endif /* CCL_PROJECT_FUNCTION_CALL_HPP */
