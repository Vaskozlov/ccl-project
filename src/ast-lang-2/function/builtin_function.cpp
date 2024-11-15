#include <ast-lang-2/function/builtin_function.hpp>

namespace astlang2::function
{
    BuiltinFunction::BuiltinFunction(
        std::vector<std::string>
            arguments_names,
        std::function<Value(interpreter::Interpreter &)>
            builtin_function)
      : Function{std::move(arguments_names)}
      , builtinFunction{std::move(builtin_function)}
    {}

    auto BuiltinFunction::doCall(interpreter::Interpreter &interpreter) const -> Value
    {
        return builtinFunction(interpreter);
    }
}// namespace astlang2::function