#ifndef CCL_PROJECT_BUILTIN_FUNCTION_HPP
#define CCL_PROJECT_BUILTIN_FUNCTION_HPP

#include <ast-lang/interpreter/function_interface.hpp>
#include <functional>

namespace astlang::interpreter
{
    class BuiltinFunction : public FunctionInterface
    {
    private:
        std::function<EvaluationResult(Interpreter &)> builtinFunction;

    public:
        explicit BuiltinFunction(
            std::vector<std::string>
                arguments_names,
            std::function<EvaluationResult(Interpreter &)>
                builtin_function);

        auto doCall(Interpreter &interpreter) -> EvaluationResult override;
    };
}// namespace astlang::interpreter

#endif /* CCL_PROJECT_BUILTIN_FUNCTION_HPP */
