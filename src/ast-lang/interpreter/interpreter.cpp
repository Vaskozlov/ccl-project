#include <ast-lang/interpreter/builtin_function.hpp>
#include <ast-lang/interpreter/interpreter.hpp>

namespace astlang::interpreter
{
    template<typename LHS_T, typename RHS_T>
    static auto createBinaryFunction(
        Interpreter &interpreter, const std::string &name, const Type &lhs_type,
        const Type &rhs_type, const Type &return_type, auto &&function) -> void
    {
        interpreter.addFunction(
            {.name = name, .returnType = return_type, .parameters = {lhs_type, rhs_type}},
            isl::makeUnique<BuiltinFunction>(
                std::vector<std::string>{"lhs", "rhs"},
                [function, return_type](Interpreter &interpreter) {
                    auto *lhs_ptr = astlang::observe<LHS_T>(interpreter.read("lhs"));
                    auto *rhs_ptr = astlang::observe<RHS_T>(interpreter.read("rhs"));

                    return EvaluationResult{
                        isl::UniqueAny{function(*lhs_ptr, *rhs_ptr)}, return_type};
                }));
    }

    template<typename T>
    static auto createPrintFunction(
        Interpreter &interpreter, const std::string &name, const Type &type) -> void
    {
        interpreter.addFunction(
            {.name = name, .returnType = Type::VOID, .parameters = {type}},
            isl::makeUnique<BuiltinFunction>(
                std::vector<std::string>{"value"}, [](Interpreter &interpreter) {
                    auto *value_ptr = astlang::observe<T>(interpreter.read("value"));
                    fmt::println("{}", *value_ptr);

                    return EvaluationResult{
                        .value = std::nullopt,
                        .type = Type::VOID,
                    };
                }));
    }

    template<typename FROM>
    static auto singleArgumentFunction(
        Interpreter &interpreter, const std::string &name, const Type &value_type,
        const Type &return_type, auto &&function) -> void
    {
        interpreter.addFunction(
            {.name = name, .returnType = return_type, .parameters = {value_type}},
            isl::makeUnique<BuiltinFunction>(
                std::vector<std::string>{"value"},
                [function, return_type](Interpreter &interpreter) {
                    auto *value_ptr = astlang::observe<FROM>(interpreter.read("value"));

                    return EvaluationResult{isl::UniqueAny{function(*value_ptr)}, return_type};
                }));
    }

    Interpreter::Interpreter(ccl::parser::reader::RulesConstructor &rules_constructor)
      : constructor(rules_constructor)
    {
        createBinaryFunction<isl::ssize_t, isl::ssize_t>(
            *this, "__addition__", Type::INT, Type::INT, Type::INT, std::plus{});

        createBinaryFunction<std::size_t, std::size_t>(
            *this, "__addition__", Type::UINT, Type::UINT, Type::UINT, std::plus{});

        createBinaryFunction<isl::ssize_t, isl::ssize_t>(
            *this, "__subtraction__", Type::INT, Type::INT, Type::INT, std::minus{});

        createBinaryFunction<std::size_t, std::size_t>(
            *this, "__subtraction__", Type::UINT, Type::UINT, Type::UINT, std::minus{});

        createBinaryFunction<isl::ssize_t, isl::ssize_t>(
            *this, "__multiplication__", Type::INT, Type::INT, Type::INT, std::multiplies{});

        createBinaryFunction<std::size_t, std::size_t>(
            *this, "__multiplication__", Type::UINT, Type::UINT, Type::UINT, std::multiplies{});

        createBinaryFunction<isl::ssize_t, isl::ssize_t>(
            *this, "__division__", Type::INT, Type::INT, Type::INT, std::divides{});

        createBinaryFunction<std::size_t, std::size_t>(
            *this, "__division__", Type::UINT, Type::UINT, Type::UINT, std::divides{});

        createBinaryFunction<isl::ssize_t, isl::ssize_t>(
            *this, "__equal__", Type::INT, Type::INT, Type::BOOL, std::equal_to{});

        createBinaryFunction<isl::ssize_t, isl::ssize_t>(
            *this, "__not_equal__", Type::INT, Type::INT, Type::BOOL, std::not_equal_to{});

        createBinaryFunction<isl::ssize_t, isl::ssize_t>(
            *this, "__less__", Type::INT, Type::INT, Type::BOOL, std::less{});

        createBinaryFunction<isl::ssize_t, isl::ssize_t>(
            *this, "__less_equal__", Type::INT, Type::INT, Type::BOOL, std::less_equal{});

        createBinaryFunction<isl::ssize_t, isl::ssize_t>(
            *this, "__greater__", Type::INT, Type::INT, Type::BOOL, std::greater{});

        createBinaryFunction<isl::ssize_t, isl::ssize_t>(
            *this, "__greater_equal__", Type::INT, Type::INT, Type::BOOL, std::greater_equal{});

        createBinaryFunction<bool, bool>(
            *this, "__logical_and__", Type::BOOL, Type::BOOL, Type::BOOL, std::logical_and{});

        createBinaryFunction<bool, bool>(
            *this, "__logical_or__", Type::BOOL, Type::BOOL, Type::BOOL, std::logical_or{});

        createBinaryFunction<isl::ssize_t, isl::ssize_t>(
            *this, "__bitwise_and", Type::INT, Type::INT, Type::INT, std::bit_and{});

        createBinaryFunction<isl::ssize_t, isl::ssize_t>(
            *this, "__bitwise_or", Type::INT, Type::INT, Type::INT, std::bit_or{});

        createBinaryFunction<isl::ssize_t, isl::ssize_t>(
            *this, "__bitwise_xor", Type::INT, Type::INT, Type::INT, std::bit_xor{});

        createBinaryFunction<std::size_t, std::size_t>(
            *this, "__bitwise_and", Type::UINT, Type::UINT, Type::UINT, std::bit_and{});

        createBinaryFunction<std::size_t, std::size_t>(
            *this, "__bitwise_or", Type::UINT, Type::UINT, Type::UINT, std::bit_or{});

        createBinaryFunction<std::size_t, std::size_t>(
            *this, "__bitwise_xor", Type::UINT, Type::UINT, Type::UINT, std::bit_xor{});

        createPrintFunction<bool>(*this, "print", Type::BOOL);
        createPrintFunction<isl::ssize_t>(*this, "print", Type::INT);
        createPrintFunction<std::size_t>(*this, "print", Type::UINT);
        createPrintFunction<std::string>(*this, "print", Type::STRING);

        singleArgumentFunction<bool>(*this, "bool", Type::INT, Type::BOOL, [](isl::ssize_t value) {
            return value != 0;
        });

        singleArgumentFunction<bool>(*this, "bool", Type::UINT, Type::BOOL, [](isl::ssize_t value) {
            return value != 0;
        });

        singleArgumentFunction<isl::ssize_t>(
            *this, "__negation__", Type::INT, Type::INT, std::negate{});

        singleArgumentFunction<std::size_t>(
            *this, "__positive__", Type::INT, Type::INT, [](std::size_t value) {
                return value;
            });

        singleArgumentFunction<std::size_t>(
            *this, "__positive__", Type::UINT, Type::UINT, [](std::size_t value) {
                return value;
            });
    }

    auto Interpreter::addFunction(
        FunctionIdentification identification,
        isl::UniquePtr<FunctionInterface>
            function) -> void
    {
        functions.addFunction(std::move(identification), std::move(function));
    }

    auto Interpreter::call(const std::string &name, FunctionCallArguments arguments)
        -> EvaluationResult
    {
        auto conversion_range = std::views::transform(arguments, [](const auto &argument) {
            return argument.type;
        });

        auto function_identification = FunctionIdentification{
            .name = name,
            .returnType = Type::ERROR,
            .parameters = std::vector<Type>{conversion_range.begin(), conversion_range.end()},
        };

        auto *function = functions.getFunction(function_identification);
        return function->call(*this, std::move(arguments));
    }
}// namespace astlang::interpreter
