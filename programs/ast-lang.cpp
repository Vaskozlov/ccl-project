#include <ast-lang/ast/global_declarations.hpp>
#include <ast-lang/ast/node.hpp>
#include <ast-lang/interpreter/interpreter.hpp>
#include <ccl/lexer/tokenizer.hpp>
#include <ccl/parser/dot/dot_repr.hpp>
#include <ccl/parser/lr/lr_parser.hpp>
#include <ccl/parser/rules_reader/rules_reader.hpp>
#include <isl/io.hpp>

using namespace ccl::lexer;
using namespace astlang::interpreter;

auto main() -> int
{
    using namespace ccl;
    using namespace ccl::parser;

    auto filename = std::filesystem::path(
        "/Users/vaskozlov/CLionProjects/ccl-project/include/ast-lang/ast-lang.cclp");

    reader::RulesReader reader(isl::io::readFile(filename), filename.string());

    auto &constructor = reader.getParserBuilder();
    auto &lexer = constructor.getLexicalAnalyzer();
    auto to_str = constructor.getIdToNameTranslationFunction();

    auto input =
        isl::io::readFile("/Users/vaskozlov/CLionProjects/ccl-project/programs/test.astlang");
    auto tokenizer = lexer.getTokenizer(input);

    auto lr_parser = constructor.buildLr1();
    auto [lifetime_manager, node] = lr_parser.parse(tokenizer);

    auto dot_repr = dot::createDotRepresentation({node}, to_str);

    isl::io::writeToFile(
        "/Users/vaskozlov/CLionProjects/ccl-project/cmake-build-debug-clang/test.dot", dot_repr);

    auto *result_as_sequence = dynamic_cast<ast::NodeOfNodes *>(node);

    auto global_declarations_node = isl::makeUnique<astlang::ast::GlobalDeclarations>(
        node->getType(), std::move(result_as_sequence->getNodes()));

    astlang::ast::Node::convertCclTreeToAstlang(constructor, global_declarations_node.get());

    auto interpreter = Interpreter{constructor};

    // global_declarations_node->print("", false, to_str);
    global_declarations_node->runRecursiveOptimization();

    // global_declarations_node->print("", false, to_str);
    global_declarations_node->compute(interpreter);

    return 0;
}
