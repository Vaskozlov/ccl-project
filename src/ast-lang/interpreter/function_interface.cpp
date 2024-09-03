#include <ast-lang/interpreter/function_interface.hpp>
#include <ast-lang/interpreter/interpreter.hpp>

namespace astlang::interpreter
{
    FunctionInterface::FunctionInterface(std::vector<std::string> arguments_names)
      : argumentsNames{std::move(arguments_names)}
    {}

    auto FunctionInterface::call(Interpreter &interpreter, FunctionCallArguments arguments) const
        -> EvaluationResult
    {
        auto scope_life = interpreter.createsHardScope();

        for (size_t i = 0; i != arguments.size(); ++i) {
            interpreter.createVariable(argumentsNames[i], std::move(arguments[i]));
        }

        return doCall(interpreter);
    }
}// namespace astlang::interpreter