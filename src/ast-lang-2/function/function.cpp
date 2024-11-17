#include <ast-lang-2/function/function.hpp>
#include <ast-lang-2/interpreter/interpreter.hpp>

namespace astlang2::function
{
    auto Function::call(
        interpreter::Interpreter &interpreter, const isl::SmallVector<Value, 4> &arguments) const
        -> Value
    {
        auto scope = interpreter.createsHardScope();

        for (u32 i = 0; i != arguments.size(); ++i) {
            interpreter.write(argumentsNames[i], arguments[i]);
        }

        return doCall(interpreter);
    }
}// namespace astlang2::function
