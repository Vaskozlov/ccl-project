#include <ast-lang/interpreter/builtin_function.hpp>
#include <ast-lang/interpreter/interpreter.hpp>

namespace astlang::interpreter
{
    BuiltinFunction::BuiltinFunction(
        std::vector<std::string>
            arguments_names,
        std::function<EvaluationResult(Interpreter &)>
            builtin_function)
      : FunctionInterface{std::move(arguments_names)}
      , builtinFunction(std::move(builtin_function))
    {}

    auto BuiltinFunction::doCall(Interpreter &interpreter) const -> EvaluationResult
    {
        return builtinFunction(interpreter);
    }
}// namespace astlang::interpreter