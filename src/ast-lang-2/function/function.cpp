#include <ast-lang-2/function/function.hpp>
#include <ast-lang-2/interpreter/interpreter.hpp>

namespace astlang2::function
{
    Function::Function(std::vector<std::string> arguments_names)
      : argumentsNames{std::move(arguments_names)}
    {}

    auto Function::call(
        interpreter::Interpreter &interpreter, const std::vector<Value> &arguments) const -> Value
    {
        auto scope = interpreter.createsHardScope();

        for (std::size_t i = 0; i != arguments.size(); ++i) {
            interpreter.write(argumentsNames[i], arguments[i]);
        }

        return doCall(interpreter);
    }
}// namespace astlang2::function
