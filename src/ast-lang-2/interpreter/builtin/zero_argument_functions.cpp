#include <ast-lang-2/function/builtin_function.hpp>
#include <ast-lang-2/interpreter/interpreter.hpp>
#include <ast-lang-2/interpreter/single_object_utility.hpp>

namespace astlang2::interpreter
{
    template<>
    static auto createZeroArgumentFunction(
        function::FunctionsHolder &functions_holder, std::string function_name,
        ts::Type *return_type, std::invocable auto &&function) -> void
    {
        function::FunctionIdentification identification{
            .name = std::move(function_name),
            .returnType = return_type,
        };

        auto builtin_function = std::make_shared<function::BuiltinFunction>(
            isl::SmallVector<std::string, 2>{}, [function](Interpreter &interpreter) {
                return function();
            });

        functions_holder.addFunction(std::move(identification), std::move(builtin_function));
    }

    auto defaultInitializeFunctionsHolderWithZeroArgumentFunctions(
        const TypeSystem &type_system,
        function::FunctionsHolder &functions_holder) -> void
    {
        createZeroArgumentFunction(
            functions_holder, "timeNs", type_system.getInt(), [&type_system]() {
                const auto time_since_epoch =
                    std::chrono::duration_cast<std::chrono::nanoseconds>(
                        std::chrono::high_resolution_clock::now().time_since_epoch())
                        .count();

                return Value{
                    .object = std::make_shared<isl::ssize_t>(time_since_epoch),
                    .type = type_system.getInt(),
                };
            });
    }
}// namespace astlang2::interpreter