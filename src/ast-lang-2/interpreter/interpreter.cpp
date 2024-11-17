#include <ast-lang-2/function/builtin_function.hpp>
#include <ast-lang-2/interpreter/interpreter.hpp>
#include <mutex>

namespace astlang2::interpreter
{
    auto defaultInitializeFunctionsHolderWithZeroArgumentFunctions(
        const TypeSystem &type_system,
        function::FunctionsHolder &functions_holder) -> void;

    auto defaultInitializeFunctionsHolderWithSingleArgumentFunctions(
        const TypeSystem &type_system,
        function::FunctionsHolder &functions_holder) -> void;

    auto defaultInitializeFunctionsHolderWithTwoArgumentsFunctions(
        const TypeSystem &type_system,
        function::FunctionsHolder &functions_holder) -> void;

    static auto defaultInitializeFunctionsHolder(
        const TypeSystem &type_system,
        function::FunctionsHolder &functions_holder) -> void
    {
        defaultInitializeFunctionsHolderWithZeroArgumentFunctions(type_system, functions_holder);
        defaultInitializeFunctionsHolderWithSingleArgumentFunctions(type_system, functions_holder);
        defaultInitializeFunctionsHolderWithTwoArgumentsFunctions(type_system, functions_holder);
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

    auto Interpreter::callFunction(const std::string &name, const isl::SmallVector<Value, 4> &arguments)
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
