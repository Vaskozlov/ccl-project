#include <ast-lang-2/function/builtin_function.hpp>
#include <ast-lang-2/interpreter/interpreter.hpp>
#include <ast-lang-2/interpreter/single_object_utility.hpp>

namespace astlang2::interpreter
{
    auto defaultInitializeFunctionsHolderWithSingleArgumentPrintFunctions(
        const TypeSystem &type_system,
        function::FunctionsHolder &functions_holder) -> void;

    template<typename ARG>
    static auto createSingleArgumentFunction(
        function::FunctionsHolder &functions_holder, std::string function_name,
        ts::Type *value_type, ts::Type *return_type, auto &&function) -> void
    {
        function::FunctionIdentification identification{
            .name = std::move(function_name),
            .arguments = {value_type},
            .returnType = return_type,
        };

        auto builtin_function = std::make_shared<function::BuiltinFunction>(
            isl::SmallVector<std::string, 2>{"value"},
            [function, return_type](Interpreter &interpreter) {
                auto value_ptr = interpreter.read("value").object;
                return function(return_type, isl::staticPointerCast<ARG>(std::move(value_ptr)));
            });

        functions_holder.addFunction(std::move(identification), std::move(builtin_function));
    }

    template<typename T>
    static auto noArgsMethod(
        ts::Type *object_type, std::string method_name, ts::Type *return_type, auto &&function)
        -> void
    {
        function::FunctionIdentification identification{
            .name = std::move(method_name),
            .arguments = {object_type},
            .returnType = return_type,
        };

        auto builtin_function = std::make_shared<function::BuiltinFunction>(
            isl::SmallVector<std::string, 2>{"self"},
            [function, return_type](Interpreter &interpreter) {
                auto value_ptr = interpreter.read("self").object;
                return function(return_type, isl::staticPointerCast<T>(std::move(value_ptr)));
            });

        object_type->addMethod(std::move(identification), std::move(builtin_function));
    }

    auto defaultInitializeFunctionsHolderWithSingleArgumentFunctions(
        const TypeSystem &type_system,
        function::FunctionsHolder &functions_holder) -> void
    {
        createSingleArgumentFunction<bool>(
            functions_holder,
            "bool",
            type_system.getBool(),
            type_system.getBool(),
            CastOperation<bool>{});

        createSingleArgumentFunction<isl::ssize_t>(
            functions_holder,
            "bool",
            type_system.getInt(),
            type_system.getBool(),
            CastOperation<bool>{});

        createSingleArgumentFunction<isl::ssize_t>(
            functions_holder,
            "int",
            type_system.getInt(),
            type_system.getInt(),
            CastOperation<isl::ssize_t>{});

        createSingleArgumentFunction<isl::ssize_t>(
            functions_holder,
            "uint",
            type_system.getInt(),
            type_system.getUint(),
            CastOperation<std::size_t>{});

        createSingleArgumentFunction<isl::ssize_t>(
            functions_holder,
            "float",
            type_system.getInt(),
            type_system.getFloat(),
            CastOperation<std::float_t>{});

        createSingleArgumentFunction<isl::ssize_t>(
            functions_holder,
            "double",
            type_system.getInt(),
            type_system.getDouble(),
            CastOperation<std::double_t>{});

        createSingleArgumentFunction<std::size_t>(
            functions_holder,
            "bool",
            type_system.getUint(),
            type_system.getBool(),
            CastOperation<bool>{});

        createSingleArgumentFunction<std::size_t>(
            functions_holder,
            "int",
            type_system.getUint(),
            type_system.getInt(),
            CastOperation<isl::ssize_t>{});

        createSingleArgumentFunction<std::size_t>(
            functions_holder,
            "uint",
            type_system.getUint(),
            type_system.getUint(),
            CastOperation<std::size_t>{});

        createSingleArgumentFunction<std::size_t>(
            functions_holder,
            "float",
            type_system.getUint(),
            type_system.getFloat(),
            CastOperation<std::float_t>{});

        createSingleArgumentFunction<std::size_t>(
            functions_holder,
            "double",
            type_system.getUint(),
            type_system.getDouble(),
            CastOperation<std::double_t>{});

        createSingleArgumentFunction<float>(
            functions_holder,
            "int",
            type_system.getFloat(),
            type_system.getInt(),
            CastOperation<isl::ssize_t>{});

        createSingleArgumentFunction<float>(
            functions_holder,
            "uint",
            type_system.getFloat(),
            type_system.getUint(),
            CastOperation<std::size_t>{});

        createSingleArgumentFunction<float>(
            functions_holder,
            "float",
            type_system.getFloat(),
            type_system.getFloat(),
            CastOperation<std::float_t>{});

        createSingleArgumentFunction<float>(
            functions_holder,
            "double",
            type_system.getFloat(),
            type_system.getDouble(),
            CastOperation<std::double_t>{});

        createSingleArgumentFunction<double>(
            functions_holder,
            "int",
            type_system.getDouble(),
            type_system.getInt(),
            CastOperation<isl::ssize_t>{});

        createSingleArgumentFunction<double>(
            functions_holder,
            "uint",
            type_system.getDouble(),
            type_system.getUint(),
            CastOperation<std::size_t>{});

        createSingleArgumentFunction<double>(
            functions_holder,
            "float",
            type_system.getDouble(),
            type_system.getFloat(),
            CastOperation<std::float_t>{});

        createSingleArgumentFunction<double>(
            functions_holder,
            "double",
            type_system.getDouble(),
            type_system.getDouble(),
            CastOperation<std::double_t>{});

        noArgsMethod<std::string>(
            type_system.getString(), "length", type_system.getUint(),
            GetLengthOperation<std::size_t>{});

        createSingleArgumentFunction<std::string>(
            functions_holder, "string", type_system.getString(), type_system.getString(),
            CastOperation<std::string>{});

        createSingleArgumentFunction<bool>(
            functions_holder, "string", type_system.getBool(), type_system.getString(),
            ToStringOperation{});

        createSingleArgumentFunction<isl::ssize_t>(
            functions_holder, "string", type_system.getInt(), type_system.getString(),
            ToStringOperation{});

        createSingleArgumentFunction<std::size_t>(
            functions_holder, "string", type_system.getUint(), type_system.getString(),
            ToStringOperation{});

        createSingleArgumentFunction<float>(
            functions_holder, "string", type_system.getFloat(), type_system.getString(),
            ToStringOperation{});

        createSingleArgumentFunction<double>(
            functions_holder, "string", type_system.getDouble(), type_system.getString(),
            ToStringOperation{});

        createSingleArgumentFunction<isl::ssize_t>(
            functions_holder, "__negation__", type_system.getInt(), type_system.getInt(),
            NegateOperation<isl::ssize_t>{});

        createSingleArgumentFunction<isl::ssize_t>(
            functions_holder, "__positive__", type_system.getInt(), type_system.getInt(),
            PositiveOperation<isl::ssize_t>{});

        createSingleArgumentFunction<std::float_t>(
            functions_holder, "__negation__", type_system.getFloat(), type_system.getFloat(),
            NegateOperation<std::float_t>{});

        createSingleArgumentFunction<std::float_t>(
            functions_holder, "__positive__", type_system.getFloat(), type_system.getFloat(),
            PositiveOperation<std::float_t>{});

        createSingleArgumentFunction<std::double_t>(
            functions_holder, "__negation__", type_system.getDouble(), type_system.getDouble(),
            NegateOperation<std::double_t>{});

        createSingleArgumentFunction<std::double_t>(
            functions_holder, "__positive__", type_system.getDouble(), type_system.getDouble(),
            PositiveOperation<std::double_t>{});

        defaultInitializeFunctionsHolderWithSingleArgumentPrintFunctions(
            type_system, functions_holder);
    }
}// namespace astlang2::interpreter