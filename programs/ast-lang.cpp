#include <ast-lang/ast/node.hpp>
#include <ast-lang/interpreter/interpreter.hpp>
#include <ccl/lexer/tokenizer.hpp>
#include <ccl/parser/dot/dot_repr.hpp>
#include <ccl/parser/ll/gll_parser.hpp>
#include <ccl/parser/lr/glr_parser.hpp>
#include <ccl/parser/lr/lr_parser.hpp>
#include <ccl/parser/rules_reader/rules_reader.hpp>
#include <isl/io.hpp>

using namespace ccl::lexer;
using namespace astlang::interpreter;

constexpr static auto Input = isl::string_view{R"(
def f(int a, int b)
{
    if a > 10 {
        return 0 + b;
    }

    else {
        return 2 + b;
    }
}

var tmp = f(12, 3) * 10;
var print_result = println(tmp);
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

    auto *row_root = nodes.front().get();
    const auto conversion_table = astlang::ast::Node::buildConversionTable(constructor);
    row_root->cast(conversion_table);

    auto *astlang_root = dynamic_cast<astlang::ast::Node *>(row_root);
    auto output_buffer = std::string{};

    auto interpreter = Interpreter{constructor, std::back_inserter(output_buffer)};

    astlang_root->print("", false, to_str);
    astlang_root->runRecursiveOptimization();

    auto dot_repr = dot::createDotRepresentation(std::vector<ast::Node *>{astlang_root}, to_str);

    isl::io::write(
        std::filesystem::current_path().append(fmt::format("{}.dot", algorithm)), dot_repr);

    astlang_root->print("", false, to_str);
    astlang_root->compute(interpreter);

    fmt::print("{}\n", output_buffer);

    return 0;
}
