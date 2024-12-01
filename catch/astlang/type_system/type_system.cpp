#include <ast-lang-2/function/builtin_function.hpp>
#include <ast-lang-2/interpreter/interpreter.hpp>
#include <ccl/debug/debug.hpp>
#include <ccl/parser/rules_reader/rules_reader.hpp>

TEST_CASE("SimpleFunctionCall", "[TypeSystem]")
{
    using namespace astlang2;

    ccl::parser::reader::RulesReader reader(getNodeTypesMap(), getAstlangGrammar());

    auto &constructor = reader.getParserBuilder();
    auto to_str = constructor.getIdToNameTranslationFunction();
    auto output_buffer = std::string{};

    interpreter::Interpreter interpreter{constructor, std::back_inserter(output_buffer)};

    interpreter.addFunction(
        function::FunctionIdentification{
            "simple_print", {interpreter.getInt()}, interpreter.getInt()},
        std::make_unique<function::BuiltinFunction>(
            isl::SmallVector<std::string, 2>{"a"}, [](interpreter::Interpreter &interpreter) {
                fmt::println(
                    "Inside function: {}", *static_cast<int *>(interpreter.read("a").object.get()));
                return Value{AstlangObject<>{}, nullptr};
            }));

    auto scope = interpreter.createsHardScope();
    const astlang2::AstlangObject<> test_value = astlang2::AstlangObject<int>(42);

    interpreter.write("foo", Value{test_value, interpreter.getInt()});
    fmt::println("foo value {}", *static_cast<int *>(interpreter.read("foo").object.get()));

    interpreter.getFunction({.name = "simple_print", .arguments = {interpreter.getInt()}})
        ->call(interpreter, {Value{test_value, interpreter.getInt()}});
}
