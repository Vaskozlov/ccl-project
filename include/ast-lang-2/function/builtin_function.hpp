#ifndef AST_LANG_2_BUILTIN_FUNCTION_HPP
#define AST_LANG_2_BUILTIN_FUNCTION_HPP

#include <ast-lang-2/function/function.hpp>
#include <ast-lang-2/interpreter/interpreter.hpp>

namespace astlang2::function
{
    class BuiltinFunction final : public Function
    {
    private:
        std::function<Value(interpreter::Interpreter &)> builtinFunction;

    public:
        BuiltinFunction(
            const isl::SmallVector<std::string, 2> &arguments_names,
            std::function<Value(interpreter::Interpreter &)>
                builtin_function);

        auto doCall(interpreter::Interpreter &interpreter) const -> Value override;
    };
}// namespace astlang2::function

#endif /* AST_LANG_2_BUILTIN_FUNCTION_HPP */
