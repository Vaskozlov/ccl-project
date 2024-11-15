#include <ast-lang/interpreter/builtin_function.hpp>
#include <ast-lang/interpreter/interpreter.hpp>

namespace astlang::interpreter
{
    template<typename LHS_T, typename RHS_T>
    static auto createBinaryFunction(
        Interpreter &interpreter, const std::string &name, const type_system::Type *lhs_type,
        const type_system::Type *rhs_type, const type_system::Type *return_type, auto &&function)
        -> void
    {
        // interpreter.addFunction(
        //     {.name = name, .returnType = return_type, .parameters = {lhs_type, rhs_type}},
        //     std::make_unique<BuiltinFunction>(
        //         std::vector<std::string>{"lhs", "rhs"},
        //         [function, return_type](Interpreter &interpreter) {
        //             // auto *lhs_ptr = astlang::observe<LHS_T>(interpreter.read("lhs"));
        //             // auto *rhs_ptr = astlang::observe<RHS_T>(interpreter.read("rhs"));
        //
        //             // return EvaluationResult{
        //             // isl::UniqueAny{function(*lhs_ptr, *rhs_ptr)}, return_type};
        //             return EvaluationResult{};
        //         }));
    }

    template<typename T>
    static auto createPrintFunction(
        Interpreter &interpreter, const std::string &name, std::string_view fmt,
        const type_system::Type *type) -> void
    {
        // interpreter.addFunction(
        //     {.name = name,
        //      .returnType = interpreter.typeSystem.getType("void"),
        //      // .parameters = {type}},
        //     std::make_unique<BuiltinFunction>(
        //         std::vector<std::string>{"value"}, [fmt](Interpreter &interpreter) {
        //             // auto *value_ptr = astlang::observe<T>(interpreter.read("value"));
        //             // fmt::format_to(interpreter.getInserter(), fmt::runtime(fmt), *value_ptr);
        //             //
        //             // return EvaluationResult{
        //             //     .value = std::nullopt,
        //             //     .type = Type::VOID,
        //             // };
        //
        //             return EvaluationResult{};
        //         }));
    }


    template<typename FROM>
    static auto singleArgumentFunction(
        Interpreter &interpreter, const std::string &name, const type_system::Type *value_type,
        const type_system::Type *return_type, auto &&function) -> void
    {
        // interpreter.addFunction(
        //     {.name = name, .returnType = return_type, .parameters = {value_type}},
        //     std::make_unique<BuiltinFunction>(
        //         std::vector<std::string>{"value"},
        //         [function, return_type](Interpreter &interpreter) {
        //             // auto *value_ptr = astlang::observe<FROM>(interpreter.read("value"));
        //             //
        //             // return EvaluationResult{
        //             //     .value = isl::UniqueAny{function(*value_ptr)},
        //             //     .type = return_type,
        //             // };
        //
        //             return EvaluationResult{};
        //         }));
    }

    Interpreter::Interpreter(
        ccl::parser::reader::ParserBuilder &parser_builder,
        const std::back_insert_iterator<std::string>
            buffer_inserter)
      : inserter{buffer_inserter}
      , constructor(parser_builder)
    {
        auto *INT = typeSystem.getType("int");
        auto *UINT = typeSystem.getType("uint");
        auto *FLOAT = typeSystem.getType("float");
        auto *DOUBLE = typeSystem.getType("double");
        auto *BOOL = typeSystem.getType("bool");
        auto *STRING = typeSystem.getType("STRING");

        createBinaryFunction<isl::ssize_t, isl::ssize_t>(
            *this, "__addition__", INT, INT, INT, std::plus{});

        createBinaryFunction<std::size_t, std::size_t>(
            *this, "__addition__", UINT, UINT, UINT, std::plus{});

        createBinaryFunction<isl::ssize_t, isl::ssize_t>(
            *this, "__subtraction__", INT, INT, INT, std::minus{});

        createBinaryFunction<std::size_t, std::size_t>(
            *this, "__subtraction__", UINT, UINT, UINT, std::minus{});

        createBinaryFunction<isl::ssize_t, isl::ssize_t>(
            *this, "__multiplication__", INT, INT, INT, std::multiplies{});

        createBinaryFunction<std::size_t, std::size_t>(
            *this, "__multiplication__", UINT, UINT, UINT, std::multiplies{});

        createBinaryFunction<isl::ssize_t, isl::ssize_t>(
            *this, "__division__", INT, INT, INT, std::divides{});

        createBinaryFunction<std::size_t, std::size_t>(
            *this, "__division__", UINT, UINT, UINT, std::divides{});

        createBinaryFunction<isl::ssize_t, isl::ssize_t>(
            *this, "__equal__", INT, INT, BOOL, std::equal_to{});

        createBinaryFunction<isl::ssize_t, isl::ssize_t>(
            *this, "__not_equal__", INT, INT, BOOL, std::not_equal_to{});

        createBinaryFunction<isl::ssize_t, isl::ssize_t>(
            *this, "__less__", INT, INT, BOOL, std::less{});

        createBinaryFunction<isl::ssize_t, isl::ssize_t>(
            *this, "__less_equal__", INT, INT, BOOL, std::less_equal{});

        createBinaryFunction<isl::ssize_t, isl::ssize_t>(
            *this, "__greater__", INT, INT, BOOL, std::greater{});

        createBinaryFunction<isl::ssize_t, isl::ssize_t>(
            *this, "__greater_equal__", INT, INT, BOOL, std::greater_equal{});

        createBinaryFunction<bool, bool>(
            *this, "__logical_and__", BOOL, BOOL, BOOL, std::logical_and{});

        createBinaryFunction<bool, bool>(
            *this, "__logical_or__", BOOL, BOOL, BOOL, std::logical_or{});

        createBinaryFunction<isl::ssize_t, isl::ssize_t>(
            *this, "__bitwise_and", INT, INT, INT, std::bit_and{});

        createBinaryFunction<isl::ssize_t, isl::ssize_t>(
            *this, "__bitwise_or", INT, INT, INT, std::bit_or{});

        createBinaryFunction<isl::ssize_t, isl::ssize_t>(
            *this, "__bitwise_xor", INT, INT, INT, std::bit_xor{});

        createBinaryFunction<std::size_t, std::size_t>(
            *this, "__bitwise_and", UINT, UINT, UINT, std::bit_and{});

        createBinaryFunction<std::size_t, std::size_t>(
            *this, "__bitwise_or", UINT, UINT, UINT, std::bit_or{});

        createBinaryFunction<std::size_t, std::size_t>(
            *this, "__bitwise_xor", UINT, UINT, UINT, std::bit_xor{});

        createPrintFunction<bool>(*this, "print", "{}", BOOL);
        createPrintFunction<isl::ssize_t>(*this, "print", "{}", INT);
        createPrintFunction<std::size_t>(*this, "print", "{}", UINT);
        createPrintFunction<std::string>(*this, "print", "{}", STRING);

        createPrintFunction<bool>(*this, "println", "{}\n", BOOL);
        createPrintFunction<isl::ssize_t>(*this, "println", "{}\n", INT);
        createPrintFunction<std::size_t>(*this, "println", "{}\n", UINT);
        createPrintFunction<std::string>(*this, "println", "{}\n", STRING);

        singleArgumentFunction<bool>(*this, "bool", INT, BOOL, [](isl::ssize_t value) {
            return value != 0;
        });

        singleArgumentFunction<bool>(*this, "bool", UINT, BOOL, [](isl::ssize_t value) {
            return value != 0;
        });

        singleArgumentFunction<isl::ssize_t>(*this, "__negation__", INT, INT, std::negate{});

        singleArgumentFunction<std::size_t>(*this, "__positive__", INT, INT, [](std::size_t value) {
            return value;
        });

        singleArgumentFunction<std::size_t>(
            *this, "__positive__", UINT, UINT, [](std::size_t value) {
                return value;
            });

        // addFunction(
        //     FunctionIdentification{
        //         .name = "print",
        //         .returnType = Type::VOID,
        //         .parameters =
        //             {
        //                 {
        //                     Type::LIST,
        //                     {
        //                         Type::ANY,
        //                     },
        //                 },
        //             },
        //     },
        //     std::make_unique<BuiltinFunction>(
        //         std::vector<std::string>{"value"}, [](Interpreter &interpreter) {
        //             auto *value_ptr =
        //                 astlang::observe<std::vector<EvaluationResult>>(interpreter.read("value"));
        //
        //             auto output_iterator = interpreter.getInserter();
        //             fmt::format_to(output_iterator, "[");
        //
        //             for (auto &element : *value_ptr) {
        //                 auto arguments = FunctionCallArguments{};
        //                 arguments.emplace_back(
        //                     EvaluationResult{
        //                         .value = isl::UniqueAny{std::addressof(element)},
        //                         .type = element.type,
        //                         .storesReference = true,
        //                     });
        //
        //                 std::ignore = interpreter.call("print", std::move(arguments));
        //                 fmt::format_to(output_iterator, ", ");
        //             }
        //
        //             fmt::format_to(output_iterator, "]");
        //
        //             return EvaluationResult{
        //                 .value = std::nullopt,
        //                 .type = Type::VOID,
        //             };
        //         }));
    }

    auto Interpreter::addFunction(
        FunctionIdentification identification,
        std::unique_ptr<FunctionInterface>
            function) -> void
    {
        functions.addFunction(std::move(identification), std::move(function));
    }

    auto Interpreter::call(const std::string &name, FunctionCallArguments arguments)
        -> EvaluationResult
    {
        auto conversion_range = std::views::transform(arguments, [](const auto &argument) {
            return argument.instance.type;
        });

        const auto function_identification = FunctionIdentification{
            .name = name,
            .returnType = typeSystem.getType("void"),
            .parameters =
                std::vector<type_system::Type *>{conversion_range.begin(), conversion_range.end()},
        };

        const auto *function = functions.getFunction(function_identification);
        return function->call(*this, std::move(arguments));
    }
}// namespace astlang::interpreter
