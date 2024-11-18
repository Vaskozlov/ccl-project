#ifndef AST_LANG_2_FUNCTION_HPP
#define AST_LANG_2_FUNCTION_HPP

#include <ast-lang-2/ts/value.hpp>

#include "../../../../../../../Volumes/ramdisk/ccl-project/clang-brew/rel-with-deb-info/_deps/isl-src/include/isl/small_vector.hpp"

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
        isl::SmallVector<ts::Type *, 4> arguments;
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
        isl::SmallVector<std::string, 2> argumentsNames;

    public:
        explicit Function(const isl::SmallVector<std::string, 2> &arguments_names)
          : argumentsNames{arguments_names}
        {}

        virtual ~Function() = default;

        Function(Function &&other) = delete;
        Function(const Function &other) = delete;

        auto operator=(Function &&other) -> Function & = delete;
        auto operator=(const Function &other) -> Function & = delete;

        auto call(
            interpreter::Interpreter &interpreter,
            const isl::SmallVector<Value, 4> &arguments) const -> Value;

    protected:
        virtual auto doCall(interpreter::Interpreter &interpreter) const -> Value = 0;
    };
}// namespace astlang2::function

#endif /* AST_LANG_2_FUNCTION_HPP */
