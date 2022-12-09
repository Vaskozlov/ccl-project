#include "ccl/lex/analyzer_generator/analyzer_generator.hpp"
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
        10.0f;
        auto i = 10;
        i+10; // test
        auto _ = "Hello, World!" + "test\"")"_sv;

    // NOLINTBEGIN
    auto analyzer = LexicalAnalyzer(
        handler::Cmd::instance(),
        {{2, R"( [0-9]+"."[0-9]* ( [a-zA-Z_]+ [a-zA-Z0-9_]* )?p )"},
         {3, "[a-zA-Z_]+"},
         {4, "[0-9]+"},
         {5, R"(! "+")"},
         {6, R"(! ";")"},
         {7, R"(! "=")"},
         {8, R"(! "\"" ("\\\"" | ["]^)* "\"" )"},
         {9, R"(! "\n")"},
         {10, R"(! "//"[\n]^*)"}});

    auto tokenizer = analyzer.getTokenizer(text);
    // NOLINTEND

    while (const auto &token = tokenizer.yield()) {
        BOOST_ASSERT(token.getId() != BAD_TOKEN);
    }
}
