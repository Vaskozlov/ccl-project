#include <ast-lang-2/ast/core/node.hpp>
#include <ast-lang-2/interpreter/interpreter.hpp>
#include <ccl/lexer/tokenizer.hpp>
#include <ccl/parser/dot/dot_repr.hpp>
#include <ccl/parser/ll/gll_parser.hpp>
#include <ccl/parser/lr/glr_parser.hpp>
#include <ccl/parser/lr/lr_parser.hpp>
#include <ccl/parser/rules_reader/rules_reader.hpp>
#include <isl/io.hpp>

using namespace ccl::lexer;
using namespace astlang2::interpreter;

auto main(int /* argc */, char** argv) -> int {
    using namespace ccl;
    using namespace ccl::parser;

    reader::RulesReader reader(astlang2::NodeTypesMap, astlang2::AstLangGrammar);

    auto&constructor = reader.getParserBuilder();
    const auto&lexer = constructor.getLexicalAnalyzer();
    auto to_str = constructor.getIdToNameTranslationFunction();
    auto input = isl::io::read(argv[1]);

    auto tokenizer = lexer.getTokenizer(input, argv[1]);

    auto parser = constructor.buildGLL();
    auto [nodes, algorithm] = parser.parse(tokenizer);
    auto converter = std::views::transform(nodes, [](auto&node) {
        return node.get();
    });

    auto* row_root = nodes.front().get();
    row_root->print("", false, to_str);

    // auto dot_repr = dot::createDotRepresentation(std::vector<ast::Node *>{astlang_root}, to_str);

    // isl::io::write(
    // std::filesystem::current_path().append(fmt::format("{}.dot", algorithm)), dot_repr);

    const auto conversion_table =
            astlang2::ast::core::AstlangNode::buildConversionTable(constructor);

    row_root->cast(conversion_table);

    auto* astlang_root = dynamic_cast<astlang2::ast::core::AstlangNode *>(row_root);
    auto output_buffer = std::string{};

    astlang_root->optimize();
    auto interpreter = Interpreter{constructor, std::back_inserter(output_buffer)};

    astlang_root->compute(interpreter);
    fmt::print("{}", output_buffer);

    return 0;
}
