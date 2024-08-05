#ifndef CCL_PROJECT_ASTLANG_FUNCTION_INTERFACE_HPP
#define CCL_PROJECT_ASTLANG_FUNCTION_INTERFACE_HPP

#include <ast-lang/interpreter/evaluation_result.hpp>

namespace astlang::interpreter
{
    class Interpreter;

    using FunctionArguments = std::vector<Type>;
    using FunctionCallArguments = std::vector<EvaluationResult>;

    class FunctionInterface
    {
    private:
        std::vector<std::string> argumentsNames;

    public:
        explicit FunctionInterface(std::vector<std::string> arguments_names);

        virtual ~FunctionInterface() = default;

        FunctionInterface(FunctionInterface &&other) = delete;
        FunctionInterface(const FunctionInterface &other) = delete;

        auto operator=(FunctionInterface &&other) -> FunctionInterface & = delete;
        auto operator=(const FunctionInterface &other) -> FunctionInterface & = delete;

        auto call(Interpreter &interpreter, FunctionCallArguments arguments) -> EvaluationResult;

        virtual auto doCall(Interpreter &interpreter) const -> EvaluationResult = 0;
    };
}// namespace astlang::interpreter

#endif /* CCL_PROJECT_ASTLANG_FUNCTION_INTERFACE_HPP */
