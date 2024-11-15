#ifndef AST_LANG_2_FUNCTION_HPP
#define AST_LANG_2_FUNCTION_HPP

#include <ast-lang-2/ts/value.hpp>

namespace astlang2::ts
{
    class Type;
}

namespace astlang2::interpreter
{
    class Interpreter;
}

namespace astlang2::function
{
    struct FunctionIdentification
    {
        std::string name;
        std::vector<ts::Type *> arguments;
        ts::Type *returnType{};

        [[nodiscard]] auto operator==(const FunctionIdentification &other) const noexcept -> bool
        {
            return name == other.name && arguments == other.arguments;
        }

        auto operator<=>(const FunctionIdentification &other) const -> std::weak_ordering
        {
            if (const auto cmp = name <=> other.name; cmp != 0) {
                return cmp;
            }

            return arguments <=> other.arguments;
        }
    };

    class Function
    {
    private:
        std::vector<std::string> argumentsNames;

    public:
        explicit Function(std::vector<std::string> arguments_names);

        virtual ~Function() = default;

        Function(Function &&other) = delete;
        Function(const Function &other) = delete;

        auto operator=(Function &&other) -> Function & = delete;
        auto operator=(const Function &other) -> Function & = delete;

        auto call(interpreter::Interpreter &interpreter, const std::vector<Value> &arguments) const
            -> Value;

    protected:
        virtual auto doCall(interpreter::Interpreter &interpreter) const -> Value = 0;
    };
}// namespace astlang2::function

#endif /* AST_LANG_2_FUNCTION_HPP */
