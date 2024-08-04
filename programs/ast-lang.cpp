#include "ccl/parser/lr/lr_parser.hpp"
#include <ast-lang/ast/expression/expression.hpp>
#include <ast-lang/ast/function_decl/argument.hpp>
#include <ast-lang/ast/function_decl/arguments.hpp>
#include <ast-lang/ast/function_decl/declaration.hpp>
#include <ast-lang/ast/global_declarations.hpp>
#include <ast-lang/ast/node_sequence.hpp>
#include <ast-lang/ast/statements/statements.hpp>
#include <ast-lang/interpreter/builtin_function.hpp>
#include <ast-lang/interpreter/interpreter.hpp>
#include <ccl/parser/rules_reader/rules_reader.hpp>
#include <isl/io.hpp>

using namespace ccl::lexer;
using namespace astlang::interpreter;

template<typename LHS_T, typename RHS_T, typename RESULT_T>
auto createBinaryFunction(
    Interpreter &interpreter, const std::string &name, Type return_type, Type lhs_type,
    Type rhs_type, std::function<RESULT_T(const LHS_T &, const RHS_T &)> function) -> void
{
    interpreter.addFunction(
        {.name = name, .returnType = return_type, .parameters = {lhs_type, rhs_type}},
        isl::makeUnique<BuiltinFunction>(
            std::vector<std::string>{"lhs", "rhs"}, [function](Interpreter &interpreter) {
                auto *lhs_ptr = astlang::observe<LHS_T>(interpreter.read("lhs"));
                auto *rhs_ptr = astlang::observe<RHS_T>(interpreter.read("rhs"));

                return EvaluationResult{isl::UniqueAny{function(*lhs_ptr, *rhs_ptr)}, Type::INT};
            }));
}

auto main() -> int
{
    using namespace ccl;
    using namespace ccl::parser;

    auto filename = std::filesystem::path(
        "/Users/vaskozlov/CLionProjects/ccl-project/include/ast-lang/ast-lang.cclp");

    ccl::parser::reader::RulesReader reader(isl::io::readFile(filename), filename.string());

    auto &constructor = reader.getRulesConstructor();
    const auto first_item = constructor.getStartItem();
    const auto &grammar = constructor.getGrammarRulesStorage();
    auto &lexical_analyzer = constructor.getLexicalAnalyzer();
    auto input =
        isl::io::readFile("/Users/vaskozlov/CLionProjects/ccl-project/programs/test.astlang");
    auto tokenizer = lexical_analyzer.getTokenizer(input);
    auto to_str = constructor.getIdToNameTranslationFunction();

    auto lr_parser = LrParser{first_item, grammar.getEpsilon(), grammar, to_str};
    auto node = lr_parser.parse(tokenizer);

    auto *result_as_sequence = dynamic_cast<ccl::parser::ast::UnNodeSequence *>(node.get());

    auto global_declarations_node = isl::makeUnique<astlang::ast::GlobalDeclarations>(
        node->getType(), std::move(result_as_sequence->getNodes()));

    astlang::ast::NodeSequence::convertCclTreeToAstlang(
        constructor, dynamic_cast<astlang::ast::NodeSequence *>(global_declarations_node.get()));

    auto interpreter = astlang::interpreter::Interpreter{constructor};

    createBinaryFunction<isl::ssize_t, isl::ssize_t, isl::ssize_t>(
        interpreter, "__addition__", Type::INT, Type::INT, Type::INT,
        [](const auto &lhs, const auto &rhs) {
            return lhs + rhs;
        });

    createBinaryFunction<std::size_t, std::size_t, std::size_t>(
        interpreter, "__addition__", Type::UINT, Type::UINT, Type::UINT,
        [](const auto &lhs, const auto &rhs) {
            return lhs + rhs;
        });

    createBinaryFunction<isl::ssize_t, isl::ssize_t, isl::ssize_t>(
        interpreter, "__subtraction__", Type::INT, Type::INT, Type::INT,
        [](const auto &lhs, const auto &rhs) {
            return lhs - rhs;
        });

    createBinaryFunction<std::size_t, std::size_t, std::size_t>(
        interpreter, "__subtraction__", Type::UINT, Type::UINT, Type::UINT,
        [](const auto &lhs, const auto &rhs) {
            return lhs - rhs;
        });

    interpreter.addFunction(
        FunctionIdentification{
            .name = "print",
            .returnType = Type::VOID,
            .parameters = {Type::INT},
        },
        isl::makeUnique<BuiltinFunction>(
            std::vector<std::string>{"value"}, [](Interpreter &interpreter) {
                auto *value_ptr = astlang::observe<isl::ssize_t>(interpreter.read("value"));
                fmt::println("{}", *value_ptr);

                return EvaluationResult{
                    .value = std::nullopt,
                    .type = Type::VOID,
                };
            }));

    global_declarations_node->print("", false, to_str);
    global_declarations_node->compute(interpreter);

    return 0;
}
