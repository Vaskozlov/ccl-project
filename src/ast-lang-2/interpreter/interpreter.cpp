#include <ast-lang-2/function/builtin_function.hpp>
#include <ast-lang-2/interpreter/interpreter.hpp>
#include <mutex>

namespace astlang2::interpreter
{
    template<typename LHS, typename RHS>
    static auto createBinaryFunction(
        function::FunctionsHolder &functions_holder,
        std::string function_name,
        ts::Type *lhs_type,
        ts::Type *rhs_type,
        ts::Type *return_type,
        std::invocable<LHS, RHS> auto &&function) -> void
    {
        function::FunctionIdentification identification{
            .name = std::move(function_name),
            .arguments = {lhs_type, rhs_type},
            .returnType = return_type,
        };

        auto builtin_function = std::make_shared<function::BuiltinFunction>(
            std::vector<std::string>{"lhs", "rhs"},
            [function = std::forward<decltype(function)>(function),
             return_type](Interpreter &interpreter) {
                const auto lhs = interpreter.read("lhs").object;
                const auto rhs = interpreter.read("rhs").object;

                auto &&resulted_value = function(
                    *static_cast<std::remove_reference_t<LHS> *>(lhs.get()),
                    *static_cast<std::remove_reference_t<RHS> *>(rhs.get()));

                return Value{
                    .object = std::make_shared<std::remove_cvref_t<decltype(resulted_value)>>(
                        std::forward<decltype(resulted_value)>(resulted_value)),
                    .type = return_type,
                    .valueType = ValueType::R_VALUE,
                };
            });

        functions_holder.addFunction(std::move(identification), std::move(builtin_function));
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
            std::vector<std::string>{"value"}, [void_type, format](Interpreter &interpreter) {
                const auto value_ptr = interpreter.read("value").object;
                auto &value = *static_cast<std::remove_cvref_t<T> *>(value_ptr.get());

                fmt::format_to(interpreter.getOutputBuffer(), fmt::runtime(format), value);

                return Value{
                    .object = nullptr,
                    .type = void_type,
                    .valueType = ValueType::R_VALUE,
                };
            });

        functions_holder.addFunction(std::move(identification), std::move(builtin_function));
    }

    template<typename T>
    static auto createCastFunction(
        function::FunctionsHolder &functions_holder, std::string function_name,
        ts::Type *value_type, ts::Type *return_type, std::invocable<T> auto &&function) -> void
    {
        function::FunctionIdentification identification{
            .name = std::move(function_name),
            .arguments = {value_type},
            .returnType = return_type,
        };

        auto builtin_function = std::make_shared<function::BuiltinFunction>(
            std::vector<std::string>{"value"}, [return_type, function](Interpreter &interpreter) {
                const auto value_ptr = interpreter.read("value").object;

                auto &value = *static_cast<std::remove_cvref_t<T> *>(value_ptr.get());
                auto &&resulted_value = function(value);

                return Value{
                    .object = std::make_shared<std::remove_cvref_t<decltype(resulted_value)>>(
                        std::forward<decltype(resulted_value)>(resulted_value)),
                    .type = return_type,
                    .valueType = ValueType::R_VALUE,
                };
            });

        functions_holder.addFunction(std::move(identification), std::move(builtin_function));
    }

    static auto defaultInitializeFunctionsHolder(
        const TypeSystem &type_system,
        function::FunctionsHolder &functions_holder) -> void
    {
        createBinaryFunction<isl::ssize_t, isl::ssize_t>(
            functions_holder,
            "__add__",
            type_system.getInt(),
            type_system.getInt(),
            type_system.getInt(),
            std::plus{});

        createBinaryFunction<isl::ssize_t, isl::ssize_t>(
            functions_holder,
            "__sub__",
            type_system.getInt(),
            type_system.getInt(),
            type_system.getInt(),
            std::minus{});

        createBinaryFunction<isl::ssize_t, isl::ssize_t>(
            functions_holder,
            "__mul__",
            type_system.getInt(),
            type_system.getInt(),
            type_system.getInt(),
            std::multiplies{});

        createBinaryFunction<isl::ssize_t, isl::ssize_t>(
            functions_holder,
            "__div__",
            type_system.getInt(),
            type_system.getInt(),
            type_system.getInt(),
            std::divides{});

        createBinaryFunction<isl::ssize_t, isl::ssize_t>(
            functions_holder,
            "__mod__",
            type_system.getInt(),
            type_system.getInt(),
            type_system.getInt(),
            std::modulus{});

        createBinaryFunction<isl::ssize_t, isl::ssize_t>(
            functions_holder,
            "__less__",
            type_system.getInt(),
            type_system.getInt(),
            type_system.getBool(),
            std::less{});

        createBinaryFunction<isl::ssize_t, isl::ssize_t>(
            functions_holder,
            "__less_eq__",
            type_system.getInt(),
            type_system.getInt(),
            type_system.getBool(),
            std::less_equal{});

        createBinaryFunction<isl::ssize_t, isl::ssize_t>(
            functions_holder,
            "__greater__",
            type_system.getInt(),
            type_system.getInt(),
            type_system.getBool(),
            std::greater{});

        createBinaryFunction<isl::ssize_t, isl::ssize_t>(
            functions_holder,
            "__greater_eq__",
            type_system.getInt(),
            type_system.getInt(),
            type_system.getBool(),
            std::greater_equal{});

        createBinaryFunction<isl::ssize_t, isl::ssize_t>(
            functions_holder,
            "__equal__",
            type_system.getInt(),
            type_system.getInt(),
            type_system.getBool(),
            std::equal_to{});

        createBinaryFunction<isl::ssize_t, isl::ssize_t>(
            functions_holder,
            "__not_equal__",
            type_system.getInt(),
            type_system.getInt(),
            type_system.getBool(),
            std::not_equal_to{});

        createBinaryFunction<isl::ssize_t, isl::ssize_t>(
            functions_holder,
            "__bitwise_and__",
            type_system.getInt(),
            type_system.getInt(),
            type_system.getInt(),
            std::bit_and{});

        createBinaryFunction<isl::ssize_t, isl::ssize_t>(
            functions_holder,
            "__bitwise_or__",
            type_system.getInt(),
            type_system.getInt(),
            type_system.getInt(),
            std::bit_or{});

        createBinaryFunction<isl::ssize_t, isl::ssize_t>(
            functions_holder,
            "__bitwise_xor__",
            type_system.getInt(),
            type_system.getInt(),
            type_system.getInt(),
            std::bit_xor{});

        createBinaryFunction<std::size_t, std::size_t>(
            functions_holder,
            "__add__",
            type_system.getUint(),
            type_system.getUint(),
            type_system.getUint(),
            std::plus{});

        createBinaryFunction<std::size_t, std::size_t>(
            functions_holder,
            "__sub__",
            type_system.getUint(),
            type_system.getUint(),
            type_system.getUint(),
            std::minus{});

        createBinaryFunction<std::size_t, std::size_t>(
            functions_holder,
            "__mul__",
            type_system.getUint(),
            type_system.getUint(),
            type_system.getUint(),
            std::multiplies{});

        createBinaryFunction<std::size_t, std::size_t>(
            functions_holder,
            "__div__",
            type_system.getUint(),
            type_system.getUint(),
            type_system.getUint(),
            std::divides{});

        createBinaryFunction<std::size_t, std::size_t>(
            functions_holder,
            "__mod__",
            type_system.getUint(),
            type_system.getUint(),
            type_system.getUint(),
            std::modulus{});

        createBinaryFunction<std::size_t, std::size_t>(
            functions_holder,
            "__less__",
            type_system.getUint(),
            type_system.getUint(),
            type_system.getBool(),
            std::less{});

        createBinaryFunction<std::size_t, std::size_t>(
            functions_holder,
            "__less_eq__",
            type_system.getUint(),
            type_system.getUint(),
            type_system.getBool(),
            std::less_equal{});

        createBinaryFunction<std::size_t, std::size_t>(
            functions_holder,
            "__greater__",
            type_system.getUint(),
            type_system.getUint(),
            type_system.getBool(),
            std::greater{});

        createBinaryFunction<std::size_t, std::size_t>(
            functions_holder,
            "__greater_eq__",
            type_system.getUint(),
            type_system.getUint(),
            type_system.getBool(),
            std::greater_equal{});

        createBinaryFunction<std::size_t, std::size_t>(
            functions_holder,
            "__equal__",
            type_system.getUint(),
            type_system.getUint(),
            type_system.getBool(),
            std::equal_to{});

        createBinaryFunction<std::size_t, std::size_t>(
            functions_holder,
            "__not_equal__",
            type_system.getUint(),
            type_system.getUint(),
            type_system.getBool(),
            std::not_equal_to{});

        createBinaryFunction<std::size_t, std::size_t>(
            functions_holder,
            "__bitwise_and__",
            type_system.getUint(),
            type_system.getUint(),
            type_system.getUint(),
            std::bit_and{});

        createBinaryFunction<std::size_t, std::size_t>(
            functions_holder,
            "__bitwise_or__",
            type_system.getUint(),
            type_system.getUint(),
            type_system.getUint(),
            std::bit_or{});

        createBinaryFunction<std::size_t, std::size_t>(
            functions_holder,
            "__bitwise_xor__",
            type_system.getUint(),
            type_system.getUint(),
            type_system.getUint(),
            std::bit_xor{});

        createBinaryFunction<float, float>(
            functions_holder,
            "__add__",
            type_system.getFloat(),
            type_system.getFloat(),
            type_system.getFloat(),
            std::plus{});

        createBinaryFunction<float, float>(
            functions_holder,
            "__sub__",
            type_system.getFloat(),
            type_system.getFloat(),
            type_system.getFloat(),
            std::minus{});

        createBinaryFunction<float, float>(
            functions_holder,
            "__mul__",
            type_system.getFloat(),
            type_system.getFloat(),
            type_system.getFloat(),
            std::multiplies{});

        createBinaryFunction<float, float>(
            functions_holder,
            "__div__",
            type_system.getFloat(),
            type_system.getFloat(),
            type_system.getFloat(),
            std::divides{});

        createBinaryFunction<float, float>(
            functions_holder,
            "__less__",
            type_system.getFloat(),
            type_system.getFloat(),
            type_system.getBool(),
            std::less{});

        createBinaryFunction<float, float>(
            functions_holder,
            "__less_eq__",
            type_system.getFloat(),
            type_system.getFloat(),
            type_system.getBool(),
            std::less_equal{});

        createBinaryFunction<float, float>(
            functions_holder,
            "__greater__",
            type_system.getFloat(),
            type_system.getFloat(),
            type_system.getBool(),
            std::greater{});

        createBinaryFunction<float, float>(
            functions_holder,
            "__greater_eq__",
            type_system.getFloat(),
            type_system.getFloat(),
            type_system.getBool(),
            std::greater_equal{});

        createBinaryFunction<float, float>(
            functions_holder,
            "__equal__",
            type_system.getFloat(),
            type_system.getFloat(),
            type_system.getBool(),
            std::equal_to{});

        createBinaryFunction<float, float>(
            functions_holder,
            "__not_equal__",
            type_system.getFloat(),
            type_system.getFloat(),
            type_system.getBool(),
            std::not_equal_to{});

        createBinaryFunction<double, double>(
            functions_holder,
            "__add__",
            type_system.getDouble(),
            type_system.getDouble(),
            type_system.getDouble(),
            std::plus{});

        createBinaryFunction<double, double>(
            functions_holder,
            "__sub__",
            type_system.getDouble(),
            type_system.getDouble(),
            type_system.getDouble(),
            std::minus{});

        createBinaryFunction<double, double>(
            functions_holder,
            "__mul__",
            type_system.getDouble(),
            type_system.getDouble(),
            type_system.getDouble(),
            std::multiplies{});

        createBinaryFunction<double, double>(
            functions_holder,
            "__div__",
            type_system.getDouble(),
            type_system.getDouble(),
            type_system.getDouble(),
            std::divides{});

        createBinaryFunction<double, double>(
            functions_holder,
            "__less__",
            type_system.getDouble(),
            type_system.getDouble(),
            type_system.getDouble(),
            std::less{});

        createBinaryFunction<double, double>(
            functions_holder,
            "__less_eq__",
            type_system.getDouble(),
            type_system.getDouble(),
            type_system.getBool(),
            std::less_equal{});

        createBinaryFunction<double, double>(
            functions_holder,
            "__greater__",
            type_system.getDouble(),
            type_system.getDouble(),
            type_system.getBool(),
            std::greater{});

        createBinaryFunction<double, double>(
            functions_holder,
            "__greater_eq__",
            type_system.getDouble(),
            type_system.getDouble(),
            type_system.getBool(),
            std::greater_equal{});

        createBinaryFunction<double, double>(
            functions_holder,
            "__equal__",
            type_system.getDouble(),
            type_system.getDouble(),
            type_system.getBool(),
            std::equal_to{});

        createBinaryFunction<double, double>(
            functions_holder,
            "__not_equal__",
            type_system.getDouble(),
            type_system.getDouble(),
            type_system.getBool(),
            std::not_equal_to{});

        createBinaryFunction<std::string, std::string>(
            functions_holder,
            "__add__",
            type_system.getString(),
            type_system.getString(),
            type_system.getString(),
            std::plus{});

        createBinaryFunction<std::string, std::string>(
            functions_holder,
            "__less__",
            type_system.getString(),
            type_system.getString(),
            type_system.getBool(),
            std::less{});

        createBinaryFunction<std::string, std::string>(
            functions_holder,
            "__less_eq__",
            type_system.getString(),
            type_system.getString(),
            type_system.getBool(),
            std::less_equal{});

        createBinaryFunction<std::string, std::string>(
            functions_holder,
            "__greater__",
            type_system.getString(),
            type_system.getString(),
            type_system.getBool(),
            std::greater{});

        createBinaryFunction<std::string, std::string>(
            functions_holder,
            "__greater_eq__",
            type_system.getString(),
            type_system.getString(),
            type_system.getBool(),
            std::greater_equal{});

        createBinaryFunction<std::string, std::string>(
            functions_holder,
            "__equal__",
            type_system.getString(),
            type_system.getString(),
            type_system.getBool(),
            std::equal_to{});

        createBinaryFunction<std::string, std::string>(
            functions_holder,
            "__not_equal__",
            type_system.getString(),
            type_system.getString(),
            type_system.getBool(),
            std::not_equal_to{});

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

        createCastFunction<isl::ssize_t>(
            functions_holder,
            "bool",
            type_system.getInt(),
            type_system.getBool(),
            [](const isl::ssize_t value) {
                return value != 0;
            });

        createCastFunction<isl::ssize_t>(
            functions_holder,
            "int",
            type_system.getInt(),
            type_system.getInt(),
            [](const isl::ssize_t value) {
                return static_cast<isl::ssize_t>(value);
            });

        createCastFunction<isl::ssize_t>(
            functions_holder,
            "uint",
            type_system.getInt(),
            type_system.getUint(),
            [](const isl::ssize_t value) {
                return static_cast<std::size_t>(value);
            });

        createCastFunction<isl::ssize_t>(
            functions_holder,
            "float",
            type_system.getInt(),
            type_system.getFloat(),
            [](const isl::ssize_t value) {
                return static_cast<float>(value);
            });

        createCastFunction<isl::ssize_t>(
            functions_holder,
            "double",
            type_system.getInt(),
            type_system.getDouble(),
            [](const isl::ssize_t value) {
                return static_cast<double>(value);
            });

        createCastFunction<std::size_t>(
            functions_holder,
            "bool",
            type_system.getUint(),
            type_system.getBool(),
            [](const std::size_t value) {
                return value != 0;
            });

        createCastFunction<std::size_t>(
            functions_holder,
            "int",
            type_system.getUint(),
            type_system.getInt(),
            [](const std::size_t value) {
                return static_cast<isl::ssize_t>(value);
            });

        createCastFunction<std::size_t>(
            functions_holder,
            "uint",
            type_system.getUint(),
            type_system.getUint(),
            [](const std::size_t value) {
                return static_cast<std::size_t>(value);
            });

        createCastFunction<std::size_t>(
            functions_holder,
            "float",
            type_system.getUint(),
            type_system.getFloat(),
            [](const std::size_t value) {
                return static_cast<float>(value);
            });

        createCastFunction<std::size_t>(
            functions_holder,
            "double",
            type_system.getUint(),
            type_system.getDouble(),
            [](const std::size_t value) {
                return static_cast<double>(value);
            });

        createCastFunction<float>(
            functions_holder,
            "int",
            type_system.getFloat(),
            type_system.getInt(),
            [](const float value) {
                return static_cast<isl::ssize_t>(value);
            });

        createCastFunction<float>(
            functions_holder,
            "uint",
            type_system.getFloat(),
            type_system.getUint(),
            [](const float value) {
                return static_cast<std::size_t>(value);
            });

        createCastFunction<float>(
            functions_holder,
            "float",
            type_system.getFloat(),
            type_system.getFloat(),
            [](const float value) {
                return static_cast<float>(value);
            });

        createCastFunction<float>(
            functions_holder,
            "double",
            type_system.getFloat(),
            type_system.getDouble(),
            [](const float value) {
                return static_cast<double>(value);
            });

        createCastFunction<double>(
            functions_holder,
            "int",
            type_system.getDouble(),
            type_system.getInt(),
            [](const double value) {
                return static_cast<isl::ssize_t>(value);
            });

        createCastFunction<double>(
            functions_holder,
            "uint",
            type_system.getDouble(),
            type_system.getUint(),
            [](const double value) {
                return static_cast<std::size_t>(value);
            });

        createCastFunction<double>(
            functions_holder,
            "float",
            type_system.getDouble(),
            type_system.getFloat(),
            [](const double value) {
                return static_cast<float>(value);
            });

        createCastFunction<double>(
            functions_holder,
            "double",
            type_system.getDouble(),
            type_system.getDouble(),
            [](const double value) {
                return static_cast<double>(value);
            });
    }

    static auto getDefaultFunctionsHolder(const TypeSystem &type_system)
        -> function::FunctionsHolder
    {
        static function::FunctionsHolder functions_holder;
        static bool initialized = false;
        static std::mutex mutex;

        if (!initialized) {
            const std::scoped_lock lock{mutex};

            if (initialized) {
                return functions_holder;
            }

            defaultInitializeFunctionsHolder(type_system, functions_holder);
            initialized = true;
        }

        return functions_holder;
    }

    Interpreter::Interpreter(
        ccl::parser::reader::ParserBuilder &parser_builder,
        const std::back_insert_iterator<std::string>
            buffer_inserter)
      : FunctionsHolder{getDefaultFunctionsHolder(*this)}
      , constructor{parser_builder}
      , outputBuffer{buffer_inserter}
    {}

    auto Interpreter::callFunction(const std::string &name, const std::vector<Value> &arguments)
        -> Value
    {
        function::FunctionIdentification function_identification{.name = name};

        for (const auto &arg : arguments) {
            function_identification.arguments.emplace_back(arg.type);
        }

        const auto *function = getFunction(function_identification);
        return function->call(*this, arguments);
    }
}// namespace astlang2::interpreter
