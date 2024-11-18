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

        functions_holder.addFunction(
            function::FunctionIdentification{
                .name = "vec2",
                .arguments = {type_system.getDouble(), type_system.getDouble()},
                .returnType = type_system.getVec2()},
            std::make_shared<function::BuiltinFunction>(
                isl::SmallVector<std::string, 2>{"x", "y"},
                [](Interpreter &interpreter) {
                    auto x_ptr = interpreter.read("x");
                    auto y_ptr = interpreter.read("y");
                    auto x_value = *static_cast<double *>(x_ptr.object.get());
                    auto y_value = *static_cast<double *>(y_ptr.object.get());

                    auto vec2 = Instance{
                        {
                            "x",
                            Value{
                                .object = std::make_shared<double>(x_value),
                                .type = interpreter.getDouble(),
                                .valueType = ValueType::L_VALUE,
                            },
                        },
                        {
                            "y",
                            Value{
                                .object = std::make_shared<double>(y_value),
                                .type = interpreter.getDouble(),
                                .valueType = ValueType::L_VALUE,
                            },
                        },
                    };

                    return Value{
                        .object = std::make_shared<Instance>(std::move(vec2)),
                        .type = interpreter.getVec2(),
                    };
                }));

        functions_holder.addFunction(
            function::FunctionIdentification{
                .name = "vec3",
                .arguments =
                    {type_system.getDouble(), type_system.getDouble(), type_system.getDouble()},
                .returnType = type_system.getVec3()},
            std::make_shared<function::BuiltinFunction>(
                isl::SmallVector<std::string, 2>{"x", "y", "z"},
                [](Interpreter &interpreter) {
                    auto x_ptr = interpreter.read("x");
                    auto y_ptr = interpreter.read("y");
                    auto z_ptr = interpreter.read("z");
                    auto x_value = *static_cast<double *>(x_ptr.object.get());
                    auto y_value = *static_cast<double *>(y_ptr.object.get());
                    auto z_value = *static_cast<double *>(z_ptr.object.get());

                    auto vec3 = Instance{
                        {
                            "x",
                            Value{
                                .object = std::make_shared<double>(x_value),
                                .type = interpreter.getDouble(),
                                .valueType = ValueType::L_VALUE,
                            },
                        },
                        {
                            "y",
                            Value{
                                .object = std::make_shared<double>(y_value),
                                .type = interpreter.getDouble(),
                                .valueType = ValueType::L_VALUE,
                            },
                        },
                        {
                            "z",
                            Value{
                                .object = std::make_shared<double>(z_value),
                                .type = interpreter.getDouble(),
                                .valueType = ValueType::L_VALUE,
                            },
                        },
                    };

                    return Value{
                        .object = std::make_shared<Instance>(std::move(vec3)),
                        .type = interpreter.getVec3(),
                    };
                }));
    }
}// namespace astlang2::interpreter