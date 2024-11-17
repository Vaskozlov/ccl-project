#include <ast-lang-2/function/builtin_function.hpp>
#include <ast-lang-2/interpreter/interpreter.hpp>
#include <ast-lang-2/interpreter/two_objects_utility.hpp>

namespace astlang2::interpreter
{
    template<typename LHS, typename RHS>
    static auto createBinaryFunction(
        function::FunctionsHolder &functions_holder,
        std::string function_name,
        ts::Type *lhs_type,
        ts::Type *rhs_type,
        ts::Type *return_type,
        std::invocable<ts::Type *, std::shared_ptr<LHS>, std::shared_ptr<RHS>> auto &&function)
        -> void
    {
        function::FunctionIdentification identification{
            .name = std::move(function_name),
            .arguments = {lhs_type, rhs_type},
            .returnType = return_type,
        };

        auto builtin_function = std::make_shared<function::BuiltinFunction>(
            isl::SmallVector<std::string, 2>{"lhs", "rhs"},
            [function = std::forward<decltype(function)>(function),
             return_type](Interpreter &interpreter) {
                auto lhs = interpreter.read("lhs").object;
                auto rhs = interpreter.read("rhs").object;

                return function(
                    return_type,
                    std::static_pointer_cast<LHS>(std::move(lhs)),
                    std::static_pointer_cast<RHS>(std::move(rhs)));
            });

        functions_holder.addFunction(std::move(identification), std::move(builtin_function));
    }

    auto defaultInitializeFunctionsHolderWithTwoArgumentsFunctions(
        const TypeSystem &type_system,
        function::FunctionsHolder &functions_holder) -> void
    {
        createBinaryFunction<isl::ssize_t, isl::ssize_t>(
            functions_holder,
            "__add__",
            type_system.getInt(),
            type_system.getInt(),
            type_system.getInt(),
            Plus{});

        createBinaryFunction<isl::ssize_t, isl::ssize_t>(
            functions_holder,
            "__sub__",
            type_system.getInt(),
            type_system.getInt(),
            type_system.getInt(),
            Minus{});

        createBinaryFunction<isl::ssize_t, isl::ssize_t>(
            functions_holder,
            "__mul__",
            type_system.getInt(),
            type_system.getInt(),
            type_system.getInt(),
            Multiplies{});

        createBinaryFunction<isl::ssize_t, isl::ssize_t>(
            functions_holder,
            "__div__",
            type_system.getInt(),
            type_system.getInt(),
            type_system.getInt(),
            Divides{});

        createBinaryFunction<isl::ssize_t, isl::ssize_t>(
            functions_holder,
            "__mod__",
            type_system.getInt(),
            type_system.getInt(),
            type_system.getInt(),
            Modulus{});

        createBinaryFunction<isl::ssize_t, isl::ssize_t>(
            functions_holder,
            "__less__",
            type_system.getInt(),
            type_system.getInt(),
            type_system.getBool(),
            Less{});

        createBinaryFunction<isl::ssize_t, isl::ssize_t>(
            functions_holder,
            "__less_eq__",
            type_system.getInt(),
            type_system.getInt(),
            type_system.getBool(),
            LessEqual{});

        createBinaryFunction<isl::ssize_t, isl::ssize_t>(
            functions_holder,
            "__greater__",
            type_system.getInt(),
            type_system.getInt(),
            type_system.getBool(),
            Greater{});

        createBinaryFunction<isl::ssize_t, isl::ssize_t>(
            functions_holder,
            "__greater_eq__",
            type_system.getInt(),
            type_system.getInt(),
            type_system.getBool(),
            GreaterEqual{});

        createBinaryFunction<isl::ssize_t, isl::ssize_t>(
            functions_holder,
            "__equal__",
            type_system.getInt(),
            type_system.getInt(),
            type_system.getBool(),
            EqualTo{});

        createBinaryFunction<isl::ssize_t, isl::ssize_t>(
            functions_holder,
            "__not_equal__",
            type_system.getInt(),
            type_system.getInt(),
            type_system.getBool(),
            NotEqualTo{});

        createBinaryFunction<isl::ssize_t, isl::ssize_t>(
            functions_holder,
            "__bitwise_and__",
            type_system.getInt(),
            type_system.getInt(),
            type_system.getInt(),
            BitAnd{});

        createBinaryFunction<isl::ssize_t, isl::ssize_t>(
            functions_holder,
            "__bitwise_or__",
            type_system.getInt(),
            type_system.getInt(),
            type_system.getInt(),
            BitOr{});

        createBinaryFunction<isl::ssize_t, isl::ssize_t>(
            functions_holder,
            "__bitwise_xor__",
            type_system.getInt(),
            type_system.getInt(),
            type_system.getInt(),
            BitXor{});

        createBinaryFunction<std::size_t, std::size_t>(
            functions_holder,
            "__add__",
            type_system.getUint(),
            type_system.getUint(),
            type_system.getUint(),
            Plus{});

        createBinaryFunction<std::size_t, std::size_t>(
            functions_holder,
            "__sub__",
            type_system.getUint(),
            type_system.getUint(),
            type_system.getUint(),
            Minus{});

        createBinaryFunction<std::size_t, std::size_t>(
            functions_holder,
            "__mul__",
            type_system.getUint(),
            type_system.getUint(),
            type_system.getUint(),
            Multiplies{});

        createBinaryFunction<std::size_t, std::size_t>(
            functions_holder,
            "__div__",
            type_system.getUint(),
            type_system.getUint(),
            type_system.getUint(),
            Divides{});

        createBinaryFunction<std::size_t, std::size_t>(
            functions_holder,
            "__mod__",
            type_system.getUint(),
            type_system.getUint(),
            type_system.getUint(),
            Modulus{});

        createBinaryFunction<std::size_t, std::size_t>(
            functions_holder,
            "__less__",
            type_system.getUint(),
            type_system.getUint(),
            type_system.getBool(),
            Less{});

        createBinaryFunction<std::size_t, std::size_t>(
            functions_holder,
            "__less_eq__",
            type_system.getUint(),
            type_system.getUint(),
            type_system.getBool(),
            LessEqual{});

        createBinaryFunction<std::size_t, std::size_t>(
            functions_holder,
            "__greater__",
            type_system.getUint(),
            type_system.getUint(),
            type_system.getBool(),
            Greater{});

        createBinaryFunction<std::size_t, std::size_t>(
            functions_holder,
            "__greater_eq__",
            type_system.getUint(),
            type_system.getUint(),
            type_system.getBool(),
            GreaterEqual{});

        createBinaryFunction<std::size_t, std::size_t>(
            functions_holder,
            "__equal__",
            type_system.getUint(),
            type_system.getUint(),
            type_system.getBool(),
            EqualTo{});

        createBinaryFunction<std::size_t, std::size_t>(
            functions_holder,
            "__not_equal__",
            type_system.getUint(),
            type_system.getUint(),
            type_system.getBool(),
            NotEqualTo{});

        createBinaryFunction<std::size_t, std::size_t>(
            functions_holder,
            "__bitwise_and__",
            type_system.getUint(),
            type_system.getUint(),
            type_system.getUint(),
            BitAnd{});

        createBinaryFunction<std::size_t, std::size_t>(
            functions_holder,
            "__bitwise_or__",
            type_system.getUint(),
            type_system.getUint(),
            type_system.getUint(),
            BitOr{});

        createBinaryFunction<std::size_t, std::size_t>(
            functions_holder,
            "__bitwise_xor__",
            type_system.getUint(),
            type_system.getUint(),
            type_system.getUint(),
            BitXor{});

        createBinaryFunction<float, float>(
            functions_holder,
            "__add__",
            type_system.getFloat(),
            type_system.getFloat(),
            type_system.getFloat(),
            Plus{});

        createBinaryFunction<float, float>(
            functions_holder,
            "__sub__",
            type_system.getFloat(),
            type_system.getFloat(),
            type_system.getFloat(),
            Minus{});

        createBinaryFunction<float, float>(
            functions_holder,
            "__mul__",
            type_system.getFloat(),
            type_system.getFloat(),
            type_system.getFloat(),
            Multiplies{});

        createBinaryFunction<float, float>(
            functions_holder,
            "__div__",
            type_system.getFloat(),
            type_system.getFloat(),
            type_system.getFloat(),
            Divides{});

        createBinaryFunction<float, float>(
            functions_holder,
            "__less__",
            type_system.getFloat(),
            type_system.getFloat(),
            type_system.getBool(),
            Less{});

        createBinaryFunction<float, float>(
            functions_holder,
            "__less_eq__",
            type_system.getFloat(),
            type_system.getFloat(),
            type_system.getBool(),
            LessEqual{});

        createBinaryFunction<float, float>(
            functions_holder,
            "__greater__",
            type_system.getFloat(),
            type_system.getFloat(),
            type_system.getBool(),
            Greater{});

        createBinaryFunction<float, float>(
            functions_holder,
            "__greater_eq__",
            type_system.getFloat(),
            type_system.getFloat(),
            type_system.getBool(),
            GreaterEqual{});

        createBinaryFunction<float, float>(
            functions_holder,
            "__equal__",
            type_system.getFloat(),
            type_system.getFloat(),
            type_system.getBool(),
            EqualTo{});

        createBinaryFunction<float, float>(
            functions_holder,
            "__not_equal__",
            type_system.getFloat(),
            type_system.getFloat(),
            type_system.getBool(),
            NotEqualTo{});

        createBinaryFunction<double, double>(
            functions_holder,
            "__add__",
            type_system.getDouble(),
            type_system.getDouble(),
            type_system.getDouble(),
            Plus{});

        createBinaryFunction<double, double>(
            functions_holder,
            "__sub__",
            type_system.getDouble(),
            type_system.getDouble(),
            type_system.getDouble(),
            Minus{});

        createBinaryFunction<double, double>(
            functions_holder,
            "__mul__",
            type_system.getDouble(),
            type_system.getDouble(),
            type_system.getDouble(),
            Multiplies{});

        createBinaryFunction<double, double>(
            functions_holder,
            "__div__",
            type_system.getDouble(),
            type_system.getDouble(),
            type_system.getDouble(),
            Divides{});

        createBinaryFunction<double, double>(
            functions_holder,
            "__less__",
            type_system.getDouble(),
            type_system.getDouble(),
            type_system.getDouble(),
            Less{});

        createBinaryFunction<double, double>(
            functions_holder,
            "__less_eq__",
            type_system.getDouble(),
            type_system.getDouble(),
            type_system.getBool(),
            LessEqual{});

        createBinaryFunction<double, double>(
            functions_holder,
            "__greater__",
            type_system.getDouble(),
            type_system.getDouble(),
            type_system.getBool(),
            Greater{});

        createBinaryFunction<double, double>(
            functions_holder,
            "__greater_eq__",
            type_system.getDouble(),
            type_system.getDouble(),
            type_system.getBool(),
            GreaterEqual{});

        createBinaryFunction<double, double>(
            functions_holder,
            "__equal__",
            type_system.getDouble(),
            type_system.getDouble(),
            type_system.getBool(),
            EqualTo{});

        createBinaryFunction<double, double>(
            functions_holder,
            "__not_equal__",
            type_system.getDouble(),
            type_system.getDouble(),
            type_system.getBool(),
            NotEqualTo{});

        createBinaryFunction<std::string, std::string>(
            functions_holder,
            "__add__",
            type_system.getString(),
            type_system.getString(),
            type_system.getString(),
            Plus{});

        createBinaryFunction<std::string, std::string>(
            functions_holder,
            "__less__",
            type_system.getString(),
            type_system.getString(),
            type_system.getBool(),
            Less{});

        createBinaryFunction<std::string, std::string>(
            functions_holder,
            "__less_eq__",
            type_system.getString(),
            type_system.getString(),
            type_system.getBool(),
            LessEqual{});

        createBinaryFunction<std::string, std::string>(
            functions_holder,
            "__greater__",
            type_system.getString(),
            type_system.getString(),
            type_system.getBool(),
            Greater{});

        createBinaryFunction<std::string, std::string>(
            functions_holder,
            "__greater_eq__",
            type_system.getString(),
            type_system.getString(),
            type_system.getBool(),
            GreaterEqual{});

        createBinaryFunction<std::string, std::string>(
            functions_holder,
            "__equal__",
            type_system.getString(),
            type_system.getString(),
            type_system.getBool(),
            EqualTo{});

        createBinaryFunction<std::string, std::string>(
            functions_holder,
            "__not_equal__",
            type_system.getString(),
            type_system.getString(),
            type_system.getBool(),
            NotEqualTo{});

        createBinaryFunction<bool, bool>(
            functions_holder, "__assign__", type_system.getBool(), type_system.getBool(),
            type_system.getBool(), Assign{});

        createBinaryFunction<isl::ssize_t, isl::ssize_t>(
            functions_holder, "__assign__", type_system.getInt(), type_system.getInt(),
            type_system.getInt(), Assign{});

        createBinaryFunction<std::size_t, std::size_t>(
            functions_holder, "__assign__", type_system.getUint(), type_system.getUint(),
            type_system.getUint(), Assign{});

        createBinaryFunction<float, float>(
            functions_holder, "__assign__", type_system.getFloat(), type_system.getFloat(),
            type_system.getFloat(), Assign{});

        createBinaryFunction<double, double>(
            functions_holder, "__assign__", type_system.getDouble(), type_system.getDouble(),
            type_system.getDouble(), Assign{});

        createBinaryFunction<std::string, std::string>(
            functions_holder, "__assign__", type_system.getString(), type_system.getString(),
            type_system.getString(), Assign{});
    }
}// namespace astlang2::interpreter