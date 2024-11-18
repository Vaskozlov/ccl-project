#include <ast-lang-2/function/builtin_function.hpp>
#include <ast-lang-2/interpreter/interpreter.hpp>
#include <ast-lang-2/interpreter/single_object_utility.hpp>

namespace astlang2::interpreter
{
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

    static auto constructVec2(double x, double y, const Interpreter &interpreter)
    {
        auto vec2 = Instance{
            {
                "x",
                Value{
                    .object = std::make_shared<double>(x),
                    .type = interpreter.getDouble(),
                    .valueType = ValueType::L_VALUE,
                },
            },
            {
                "y",
                Value{
                    .object = std::make_shared<double>(y),
                    .type = interpreter.getDouble(),
                    .valueType = ValueType::L_VALUE,
                },
            },
        };

        return Value{
            .object = std::make_shared<Instance>(std::move(vec2)),
            .type = interpreter.getVec2(),
        };
    }

    static auto constructVec3(double x, double y, double z, const Interpreter &interpreter)
    {
        auto vec3 = Instance{
            {
                "x",
                Value{
                    .object = std::make_shared<double>(x),
                    .type = interpreter.getDouble(),
                    .valueType = ValueType::L_VALUE,
                },
            },
            {
                "y",
                Value{
                    .object = std::make_shared<double>(y),
                    .type = interpreter.getDouble(),
                    .valueType = ValueType::L_VALUE,
                },
            },
            {
                "z",
                Value{
                    .object = std::make_shared<double>(z),
                    .type = interpreter.getDouble(),
                    .valueType = ValueType::L_VALUE,
                },
            },
        };

        return Value{
            .object = std::make_shared<Instance>(std::move(vec3)),
            .type = interpreter.getVec3(),
        };
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
                .name = "vec2", .arguments = {}, .returnType = type_system.getVec2()},
            std::make_shared<function::BuiltinFunction>(
                isl::SmallVector<std::string, 2>{}, [](const Interpreter &interpreter) {
                    return constructVec2(0.0, 0.0, interpreter);
                }));

        functions_holder.addFunction(
            function::FunctionIdentification{
                .name = "vec2",
                .arguments =
                    {
                        type_system.getDouble(),
                        type_system.getDouble(),
                    },
                .returnType = type_system.getVec2()},
            std::make_shared<function::BuiltinFunction>(
                isl::SmallVector<std::string, 2>{"x", "y"}, [](Interpreter &interpreter) {
                    const auto x_ptr = interpreter.read("x");
                    const auto y_ptr = interpreter.read("y");

                    const auto x_value = *static_cast<double *>(x_ptr.object.get());
                    const auto y_value = *static_cast<double *>(y_ptr.object.get());

                    return constructVec2(x_value, y_value, interpreter);
                }));

        functions_holder.addFunction(
            function::FunctionIdentification{
                .name = "vec3", .arguments = {}, .returnType = type_system.getVec3()},
            std::make_shared<function::BuiltinFunction>(
                isl::SmallVector<std::string, 2>{}, [](const Interpreter &interpreter) {
                    return constructVec3(0.0, 0.0, 0.0, interpreter);
                }));

        functions_holder.addFunction(
            function::FunctionIdentification{
                .name = "vec3",
                .arguments =
                    {
                        type_system.getDouble(),
                        type_system.getDouble(),
                        type_system.getDouble(),
                    },
                .returnType = type_system.getVec3()},
            std::make_shared<function::BuiltinFunction>(
                isl::SmallVector<std::string, 2>{"x", "y", "z"}, [](Interpreter &interpreter) {
                    const auto x_ptr = interpreter.read("x");
                    const auto y_ptr = interpreter.read("y");
                    const auto z_ptr = interpreter.read("z");

                    const auto x_value = *static_cast<double *>(x_ptr.object.get());
                    const auto y_value = *static_cast<double *>(y_ptr.object.get());
                    const auto z_value = *static_cast<double *>(z_ptr.object.get());

                    return constructVec3(x_value, y_value, z_value, interpreter);
                }));
    }
}// namespace astlang2::interpreter