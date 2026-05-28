#include <ast-lang-2/function/builtin_function.hpp>
#include <ast-lang-2/interpreter/interpreter.hpp>
#include <ast-lang-2/ts/value.hpp>
#include <vector>

namespace astlang2::interpreter
{
    static auto valueToString(const astlang2::Value &value, const Interpreter &interpreter)
        -> std::string
    {
        if (value.type == interpreter.getInt()) {
            return fmt::to_string(*static_cast<isl::ssize_t *>(value.object.get()));
        }
        if (value.type == interpreter.getUint()) {
            return fmt::to_string(*static_cast<std::size_t *>(value.object.get()));
        }
        if (value.type == interpreter.getBool()) {
            return *static_cast<bool *>(value.object.get()) ? "true" : "false";
        }
        if (value.type == interpreter.getFloat()) {
            return fmt::to_string(*static_cast<float *>(value.object.get()));
        }
        if (value.type == interpreter.getDouble()) {
            return fmt::to_string(*static_cast<double *>(value.object.get()));
        }
        if (value.type == interpreter.getString()) {
            return "\"" + *static_cast<std::string *>(value.object.get()) + "\"";
        }
        if (value.type == interpreter.getArray()) {
            const auto &vec =
                *static_cast<const std::vector<astlang2::Value> *>(value.object.get());
            std::string result = "[";
            for (std::size_t i = 0; i < vec.size(); ++i) {
                if (i > 0) result += ", ";
                result += valueToString(vec[i], interpreter);
            }
            result += "]";
            return result;
        }
        if (value.type == interpreter.getVec2() || value.type == interpreter.getVec3()) {
            const auto &inst = *static_cast<const Instance *>(value.object.get());
            std::string result = "(";
            bool first = true;
            for (const auto &[k, v] : inst) {
                if (!first) result += ", ";
                result += valueToString(v, interpreter);
                first = false;
            }
            result += ")";
            return result;
        }
        return "<unknown>";
    }

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

        functions_holder.addFunction(
            function::FunctionIdentification{
                .name = "print", .arguments = {type_system.getArray()},
                .returnType = type_system.getVoid()},
            std::make_shared<function::BuiltinFunction>(
                isl::SmallVector<std::string, 2>{"value"},
                [](Interpreter &interpreter) {
                    auto value_ptr = interpreter.read("value").object;
                    const auto &vec =
                        *static_cast<const std::vector<astlang2::Value> *>(value_ptr.get());
                    std::string str = "[";
                    for (std::size_t i = 0; i < vec.size(); ++i) {
                        if (i > 0) str += ", ";
                        str += valueToString(vec[i], interpreter);
                    }
                    str += "]";
                    fmt::format_to(interpreter.getOutputBuffer(), "{}", str);
                    return Value{
                        .object = AstlangObject<>{},
                        .type = interpreter.getVoid(),
                        .valueType = ValueType::R_VALUE,
                    };
                }));

        functions_holder.addFunction(
            function::FunctionIdentification{
                .name = "println", .arguments = {type_system.getArray()},
                .returnType = type_system.getVoid()},
            std::make_shared<function::BuiltinFunction>(
                isl::SmallVector<std::string, 2>{"value"},
                [](Interpreter &interpreter) {
                    auto value_ptr = interpreter.read("value").object;
                    const auto &vec =
                        *static_cast<const std::vector<astlang2::Value> *>(value_ptr.get());
                    std::string str = "[";
                    for (std::size_t i = 0; i < vec.size(); ++i) {
                        if (i > 0) str += ", ";
                        str += valueToString(vec[i], interpreter);
                    }
                    str += "]\n";
                    fmt::format_to(interpreter.getOutputBuffer(), "{}", str);
                    return Value{
                        .object = AstlangObject<>{},
                        .type = interpreter.getVoid(),
                        .valueType = ValueType::R_VALUE,
                    };
                }));

        auto addVecPrint = [&](ts::Type *vecType) {
            // print
            functions_holder.addFunction(
                function::FunctionIdentification{
                    .name = "print", .arguments = {vecType},
                    .returnType = type_system.getVoid()},
                std::make_shared<function::BuiltinFunction>(
                    isl::SmallVector<std::string, 2>{"value"},
                    [](Interpreter &interpreter) {
                        auto value_ptr = interpreter.read("value").object;
                        const auto &inst = *static_cast<const Instance *>(value_ptr.get());
                        std::string str = "(";
                        bool first = true;
                        for (const auto &[k, v] : inst) {
                            if (!first) str += ", ";
                            str += valueToString(v, interpreter);
                            first = false;
                        }
                        str += ")";
                        fmt::format_to(interpreter.getOutputBuffer(), "{}", str);
                        return Value{
                            .object = AstlangObject<>{},
                            .type = interpreter.getVoid(),
                            .valueType = ValueType::R_VALUE,
                        };
                    }));

            // println
            functions_holder.addFunction(
                function::FunctionIdentification{
                    .name = "println", .arguments = {vecType},
                    .returnType = type_system.getVoid()},
                std::make_shared<function::BuiltinFunction>(
                    isl::SmallVector<std::string, 2>{"value"},
                    [](Interpreter &interpreter) {
                        auto value_ptr = interpreter.read("value").object;
                        const auto &inst = *static_cast<const Instance *>(value_ptr.get());
                        std::string str = "(";
                        bool first = true;
                        for (const auto &[k, v] : inst) {
                            if (!first) str += ", ";
                            str += valueToString(v, interpreter);
                            first = false;
                        }
                        str += ")\n";
                        fmt::format_to(interpreter.getOutputBuffer(), "{}", str);
                        return Value{
                            .object = AstlangObject<>{},
                            .type = interpreter.getVoid(),
                            .valueType = ValueType::R_VALUE,
                        };
                    }));
        };

        addVecPrint(type_system.getVec2());
        addVecPrint(type_system.getVec3());
    }
}// namespace astlang2::interpreter