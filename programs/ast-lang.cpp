#include <ast-lang/ast/global_declarations.hpp>
#include <ast-lang/ast/node.hpp>
#include <ast-lang/interpreter/interpreter.hpp>
#include <ccl/lexer/tokenizer.hpp>
#include <ccl/parser/dot/dot_repr.hpp>
#include <ccl/parser/ll/gll.hpp>
#include <ccl/parser/lr/glr_parser.hpp>
#include <ccl/parser/lr/lr_parser.hpp>
#include <ccl/parser/rules_reader/rules_reader.hpp>
#include <isl/io.hpp>

using namespace ccl::lexer;
using namespace astlang::interpreter;

auto main() -> int
{
    using namespace ccl;
    using namespace ccl::parser;

    const auto filename = std::filesystem::path(
        "/Users/vaskozlov/CLionProjects/ccl-project/include/ast-lang/ast-lang.cclp");

    reader::RulesReader reader(isl::io::readFile(filename), filename.string());

    auto &constructor = reader.getParserBuilder();
    auto &lexer = constructor.getLexicalAnalyzer();
    auto to_str = constructor.getIdToNameTranslationFunction();

    auto input =
        isl::io::readFile("/Users/vaskozlov/CLionProjects/ccl-project/programs/test.astlang");
    auto tokenizer = lexer.getTokenizer(input);

    auto lr_parser = constructor.buildGLL();
    auto [nodes, algorithm] = lr_parser.parse(tokenizer);
    auto converter = std::views::transform(nodes, [](auto &node) {
        return node.get();
    });

    auto dot_repr = dot::createDotRepresentation(
        std::vector<ast::Node *>{converter.begin(), converter.end()}, to_str);

    isl::io::writeToFile(fmt::format("/Volumes/ramdisk/{}.dot", algorithm), dot_repr);

    auto *row_root = nodes.front().get();
    const auto conversion_table = astlang::ast::Node::buildConversionTable(constructor);
    ccl::parser::ast::Node::cast(conversion_table, row_root);

    auto *astlang_root = dynamic_cast<astlang::ast::Node *>(row_root);
    auto interpreter = Interpreter{constructor};

    astlang_root->print("", false, to_str);
    astlang_root->runRecursiveOptimization();

    astlang_root->print("", false, to_str);
    astlang_root->compute(interpreter);

    return 0;
}
