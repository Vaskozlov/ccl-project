#ifndef CCL_PROJECT_FUNCTION_DEFINITION_ARGUMENTS_HPP
#define CCL_PROJECT_FUNCTION_DEFINITION_ARGUMENTS_HPP

#include <ast-lang/ast/node_sequence.hpp>

namespace astlang::ast::function::decl
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
}// namespace astlang::ast::function::decl

#endif /* CCL_PROJECT_FUNCTION_DEFINITION_ARGUMENTS_HPP */
