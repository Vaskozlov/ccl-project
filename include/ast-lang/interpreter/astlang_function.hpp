#ifndef CCL_PROJECT_ASTLANG_FUNCTION_HPP
#define CCL_PROJECT_ASTLANG_FUNCTION_HPP

#include <ast-lang/interpreter/function_interface.hpp>
#include <ast-lang/interpreter/interpreter.hpp>

namespace astlang::interpreter
{
    class AstlangFunction : public FunctionInterface
    {
    private:
        const astlang::ast::Node *functionBody;

    public:
        explicit AstlangFunction(
            std::vector<std::string> arguments_names, const astlang::ast::Node *function_body)
          : FunctionInterface{std::move(arguments_names)}
          , functionBody(function_body)
        {}

        auto doCall(Interpreter &interpreter) const -> EvaluationResult override
        {
            return functionBody->compute(interpreter);
        }
    };
}// namespace astlang::interpreter

#endif /* CCL_PROJECT_ASTLANG_FUNCTION_HPP */
