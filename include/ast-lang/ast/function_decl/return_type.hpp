#ifndef CCL_PROJECT_RETURN_TYPE_HPP
#define CCL_PROJECT_RETURN_TYPE_HPP

#include <ast-lang/ast/node_sequence.hpp>

namespace astlang::ast::function::decl
{
    class ReturnType
      : public ccl::parser::ast::UnNodeSequence
      , public NodeSequence
    {
    public:
        using ccl::parser::ast::UnNodeSequence::NodeSequence;

        auto compute(interpreter::Interpreter &interpreter)
            -> interpreter::EvaluationResult override;
    };
}// namespace astlang::ast::function::decl

#endif /* CCL_PROJECT_RETURN_TYPE_HPP */
