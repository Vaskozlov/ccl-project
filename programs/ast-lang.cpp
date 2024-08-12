#include "ccl/parser/lr/lr_parser.hpp"
#include <ast-lang/ast/global_declarations.hpp>
#include <ast-lang/ast/node.hpp>
#include <ast-lang/interpreter/interpreter.hpp>
#include <ccl/parser/dot/dot_repr.hpp>
#include <ccl/parser/rules_reader/rules_reader.hpp>
#include <ccl/parser/visualization/graph.hpp>
#include <isl/io.hpp>

using namespace ccl::lexer;
using namespace astlang::interpreter;

auto main() -> int
{
    using namespace ccl;
    using namespace ccl::parser;

    auto filename = std::filesystem::path(
        "/Users/vaskozlov/CLionProjects/ccl-project/include/ast-lang/ast-lang.cclp");

    ccl::parser::reader::RulesReader reader(isl::io::readFile(filename), filename.string());

    auto &constructor = reader.getRulesConstructor(reader::Mode::LR);
    const auto first_item = constructor.getStartItem();
    const auto &grammar = constructor.getGrammarRulesStorage();
    auto &lexical_analyzer = constructor.getLexicalAnalyzer();
    auto input =
        isl::io::readFile("/Users/vaskozlov/CLionProjects/ccl-project/programs/test.astlang");
    auto tokenizer = lexical_analyzer.getTokenizer(input);
    auto to_str = constructor.getIdToNameTranslationFunction();

    auto lr_parser = LrParser{first_item, grammar.getEpsilon(), grammar, to_str};
    auto node = lr_parser.parse(tokenizer);
    visualization::visualize("test.dot", node.get(), to_str);

    auto *result_as_sequence = dynamic_cast<ccl::parser::ast::UnNodeSequence *>(node.get());

    auto global_declarations_node = isl::makeUnique<astlang::ast::GlobalDeclarations>(
        node->getType(), std::move(result_as_sequence->getNodes()));

    astlang::ast::Node::convertCclTreeToAstlang(
        constructor, dynamic_cast<astlang::ast::Node *>(global_declarations_node.get()));

    auto interpreter = astlang::interpreter::Interpreter{constructor};

    global_declarations_node->print("", false, to_str);
    global_declarations_node->runRecursiveOptimization();

    global_declarations_node->print("", false, to_str);
    global_declarations_node->compute(interpreter);

    return 0;
}
