#include <ccl/debug/debug_file.hpp>
#include <ccl/handler/cmd_handler.hpp>
#include <ccl/lex/lexical_analyzer.hpp>

using namespace ccl;
using namespace lex;
using namespace text;
using namespace string_view_literals;

constexpr std::array<string_view, 10> rule_names({ "EOI", "BAD TOKEN", "identifier", "number",
                                                   "addition", "column", "assignment", "string",
                                                   "new line", "comment" });

BOOST_AUTO_TEST_CASE(LexTest)
{
    auto text =
        R"(
        10
        auto i = 10;
        i+10; // test
        auto _ = "Hello, World!" + "test")"_sv;

    // NOLINTBEGIN
    auto analyzer = LexicalAnalyzer(
        handler::Cmd::instance(),
        { { 2, R"([a-zA-Z_]+)" },
          { 3, "[0-9]+" },
          { 4, R"(! "+")" },
          { 5, R"(! ";")" },
          { 6, R"(! "=")" },
          { 7, R"(! "\"" (["]^ | "\\\"")* "\"" )" },
          { 8, R"(! "\n")" },
          { 9, R"(! "//"[\n]^*)" } });

    auto tokenizer = analyzer.getTokenizer(text);
    // NOLINTEND

    auto token = tokenizer.yield();

    while (token) {
        fmt::print("{}: {}\n", rule_names.at(token.getId()), token.getRepr());
        token = tokenizer.yield();
    }
}
