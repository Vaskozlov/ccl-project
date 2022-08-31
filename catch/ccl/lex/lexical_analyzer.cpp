#include <ccl/debug/debug_file.hpp>
#include <ccl/handler/cmd_handler.hpp>
#include <ccl/lex/lexical_analyzer.hpp>

using namespace ccl;
using namespace lex;
using namespace text;
using namespace string_view_literals;

BOOST_AUTO_TEST_CASE(LexTest)
{
    auto text =
        R"(
        auto i = 10;
        i + 10; // test
        auto _ = "Hello, World!" + "test")"_sv;

    // NOLINTBEGIN
    auto analyzer = LexicalAnalyzer(
        handler::Cmd::instance(),
        { { 2, R"([a-zA-Z_]+)" },
          { 3, "[0-9]+" },
          { 4, R"( '+')" },
          { 5, R"(';')" },
          { 6, R"('=')" },
          { 7, R"("\""s)" },
          { 8, R"('\n')" },
          { 9, R"("//"co)" } });

    auto tokenizer = analyzer.getTokenizer(text);
    // NOLINTEND

    auto token = tokenizer.yield();

    while (token) {
        /*fmt::print(
            "{} {}: {}\n", token.getId(), token.getTabsAndSpaces().size(),
            token.getRepr());*/
        token = tokenizer.yield();
    }
}
