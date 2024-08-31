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

constexpr static auto Input = isl::string_view{R"(
def main(int a)
{
    if a > 10
    {
        return 0;
    }

    elif a == 2
    {
        return 1;
    }

    else {
        return 2;
    }

    return a + 10 + 3;
}

var test = main(2) * 10;
var t = print(test);
var e = print(true);
)"};

auto main() -> int
{
    using namespace ccl;
    using namespace ccl::parser;

    reader::RulesReader reader(astlang::AstLangGrammar);

    auto &constructor = reader.getParserBuilder();
    const auto &lexer = constructor.getLexicalAnalyzer();
    auto to_str = constructor.getIdToNameTranslationFunction();

    auto tokenizer = lexer.getTokenizer(Input, __FILE__);

    auto lr_parser = constructor.buildGLL();
    auto [nodes, algorithm] = lr_parser.parse(tokenizer);
    auto converter = std::views::transform(nodes, [](auto &node) {
        return node.get();
    });

    auto dot_repr = dot::createDotRepresentation(
        std::vector<ast::Node *>{converter.begin(), converter.end()}, to_str);

    isl::io::writeToFile(
        std::filesystem::current_path().append(fmt::format("{}.dot", algorithm)), dot_repr);

    auto *row_root = nodes.front().get();
    const auto conversion_table = astlang::ast::Node::buildConversionTable(constructor);
    row_root->cast(conversion_table);

    auto *astlang_root = dynamic_cast<astlang::ast::Node *>(row_root);
    auto interpreter = Interpreter{constructor};

    astlang_root->print("", false, to_str);
    astlang_root->runRecursiveOptimization();

    astlang_root->print("", false, to_str);
    astlang_root->compute(interpreter);

    return 0;
}
