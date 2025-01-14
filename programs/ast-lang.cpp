#include <ast-lang-2/ast/node.hpp>
#include <ast-lang-2/interpreter/interpreter.hpp>
#include <cassert>
#include <ccl/lexer/tokenizer.hpp>
#include <ccl/parser/dot/dot_repr.hpp>
#include <ccl/parser/ll/gll_parser.hpp>
#include <ccl/parser/lr/glr_parser.hpp>
#include <ccl/parser/lr/lr_parser.hpp>
#include <ccl/parser/rules_reader/rules_reader.hpp>
#include <isl/io.hpp>

using namespace ccl::lexer;
using namespace astlang2::interpreter;

auto main(int /* argc */, char **argv) -> int
{
    using namespace ccl;
    using namespace ccl::parser;

    reader::RulesReader reader(astlang2::getNodeTypesMap(), astlang2::getAstlangGrammar());

    auto &constructor = reader.getParserBuilder();
    const auto &lexer = constructor.getLexicalAnalyzer();
    auto to_str = constructor.getIdToNameTranslationFunction();
    auto input = isl::io::read(argv[1]);

    auto tokenizer = lexer.getTokenizer(input, argv[1]);

    auto parser = constructor.buildGLL();
    auto [nodes, algorithm] = parser.parse(tokenizer);
    assert(nodes.size() < 2);

    auto *row_root = nodes.front().get();

    auto dot_repr = dot::createDotRepresentation(std::vector<ast::Node *>{row_root}, to_str);

    isl::io::write(
        std::filesystem::current_path().append(fmt::format("{}.dot", algorithm)), dot_repr);

    const auto conversion_table = astlang2::ast::AstlangNode::buildConversionTable(constructor);

    row_root->cast(conversion_table);
    nodes.front().updateDeleter<ast::Node>();

    auto astlang_root = isl::staticPointerCast<astlang2::ast::AstlangNode>(nodes.front());

    auto output_buffer = std::string{};
    astlang2::ast::AstlangNode::exchangeIfNotNull(astlang_root, astlang_root->optimize());

    astlang_root->print("", false, to_str);
    auto interpreter = Interpreter{constructor, std::back_inserter(output_buffer)};

    astlang_root->compute(interpreter);
    fmt::print("{}", output_buffer);

    return 0;
}
