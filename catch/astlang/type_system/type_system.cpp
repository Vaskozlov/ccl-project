#include <ast-lang-2/function/builtin_function.hpp>
#include <ast-lang-2/interpreter/interpreter.hpp>
#include <ccl/debug/debug.hpp>
#include <ccl/parser/rules_reader/rules_reader.hpp>

TEST_CASE("SimpleFunctionCall", "[TypeSystem]")
{
    using namespace astlang2;

    ccl::parser::reader::RulesReader reader(astlang2::AstLangGrammar);

    auto &constructor = reader.getParserBuilder();
    const auto &lexer = constructor.getLexicalAnalyzer();
    auto to_str = constructor.getIdToNameTranslationFunction();
    auto output_buffer = std::string{};

    interpreter::Interpreter interpreter{constructor, std::back_inserter(output_buffer)};

    interpreter.addFunction(
        function::FunctionIdentification{
            "simple_print", {interpreter.getInt()}, interpreter.getInt()},
        std::make_unique<function::BuiltinFunction>(
            std::vector<std::string>{"a"}, [](interpreter::Interpreter &interpreter) {
                fmt::println(
                    "Inside function: {}", *static_cast<int *>(interpreter.read("a").value.get()));
                return Value{nullptr, nullptr};
            }));

    auto scope = interpreter.createsHardScope();
    const std::shared_ptr<void> test_value = std::make_shared<int>(42);

    interpreter.write("foo", Value{test_value, interpreter.getInt()});
    fmt::println("foo value {}", *static_cast<int *>(interpreter.read("foo").value.get()));

    interpreter.getFunction({.name = "simple_print", .arguments = {interpreter.getInt()}})
        ->call(interpreter, {Value{test_value, interpreter.getInt()}});
}
