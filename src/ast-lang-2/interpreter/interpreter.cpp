#include <ast-lang-2/function/builtin_function.hpp>
#include <ast-lang-2/interpreter/interpreter.hpp>
#include <ast-lang-2/interpreter/single_object_utility.hpp>
#include <ast-lang-2/interpreter/two_objects_utility.hpp>
#include <mutex>

namespace astlang2::interpreter
{
    static auto defaultInitializeFunctionsHolder(
        const TypeSystem &type_system,
        function::FunctionsHolder &functions_holder) -> void
    {
        defaultInitializeFunctionsHolderWithTwoArgumentsFunctions(type_system, functions_holder);
        defaultInitializeFunctionsHolderWithSingleArgumentFunctions(type_system, functions_holder);
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
