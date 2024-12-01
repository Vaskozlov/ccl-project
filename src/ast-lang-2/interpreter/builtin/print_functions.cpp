#include <ast-lang-2/function/builtin_function.hpp>
#include <ast-lang-2/interpreter/interpreter.hpp>

namespace astlang2::interpreter
{
    template<typename T>
    static auto createPintFunction(
        function::FunctionsHolder &functions_holder,
        std::string function_name,
        const std::string &format,
        ts::Type *value_type,
        ts::Type *void_type) -> void
    {
        function::FunctionIdentification identification{
            .name = std::move(function_name),
            .arguments = {value_type},
            .returnType = void_type,
        };

        auto builtin_function = std::make_shared<function::BuiltinFunction>(
            isl::SmallVector<std::string, 2>{"value"},
            [void_type, format](Interpreter &interpreter) {
                const auto value_ptr = interpreter.read("value").object;
                auto &value = *static_cast<std::remove_cvref_t<T> *>(value_ptr.get());

                fmt::format_to(interpreter.getOutputBuffer(), fmt::runtime(format), value);

                return Value{
                    .object = AstlangObject<>{},
                    .type = void_type,
                    .valueType = ValueType::R_VALUE,
                };
            });

        functions_holder.addFunction(std::move(identification), std::move(builtin_function));
    }

    auto defaultInitializeFunctionsHolderWithSingleArgumentPrintFunctions(
        const TypeSystem &type_system,
        function::FunctionsHolder &functions_holder) -> void
    {
        createPintFunction<isl::ssize_t>(
            functions_holder, "print", "{}", type_system.getInt(), type_system.getVoid());

        createPintFunction<std::size_t>(
            functions_holder, "print", "{}", type_system.getUint(), type_system.getVoid());

        createPintFunction<bool>(
            functions_holder, "print", "{}", type_system.getBool(), type_system.getVoid());

        createPintFunction<float>(
            functions_holder, "print", "{}", type_system.getFloat(), type_system.getVoid());

        createPintFunction<double>(
            functions_holder, "print", "{}", type_system.getDouble(), type_system.getVoid());

        createPintFunction<std::string>(
            functions_holder, "print", "{}", type_system.getString(), type_system.getVoid());

        createPintFunction<isl::ssize_t>(
            functions_holder, "println", "{}\n", type_system.getInt(), type_system.getVoid());

        createPintFunction<std::size_t>(
            functions_holder, "println", "{}\n", type_system.getUint(), type_system.getVoid());

        createPintFunction<bool>(
            functions_holder, "println", "{}\n", type_system.getBool(), type_system.getVoid());

        createPintFunction<float>(
            functions_holder, "println", "{}\n", type_system.getFloat(), type_system.getVoid());

        createPintFunction<double>(
            functions_holder, "println", "{}\n", type_system.getDouble(), type_system.getVoid());

        createPintFunction<std::string>(
            functions_holder, "println", "{}\n", type_system.getString(), type_system.getVoid());
    }
}// namespace astlang2::interpreter