#include <ccl/debug/debug_file.hpp>
#include <ccl/handler/cmd.hpp>
#include <ccl/lexer/tokenizer.hpp>

using namespace ccl;
using namespace lexer;
using namespace text;
using namespace isl::string_view_literals;

TEST_CASE("LexTest", "[LexicalAnalyzer]")
{
    auto text =
        R"(
        10.0f;
        auto i = 10;
        i+10; // test
        auto _ = "Hello, World!" + "test\"")"_sv;

    // NOLINTBEGIN
    auto token_id = ReservedTokenMaxValue + 1;
    auto analyzer = LexicalAnalyzer(
        handler::Cmd::instance(),
        {{token_id++, R"( [0-9]+"."[0-9]* ( [a-zA-Z_]+ [a-zA-Z0-9_]* )?p )"},
         {token_id++, "[a-zA-Z_]+"},
         {token_id++, "[0-9]+"},
         {token_id++, R"(! "+")"},
         {token_id++, R"(! ";")"},
         {token_id++, R"(! "=")"},
         {token_id++, R"(! "\"" ("\\\"" | ["]^)* "\"" )"},
         {token_id++, R"(! "\n")"},
         {token_id++, R"(! "//"[\n]^*)"}});

    auto tokenizer = analyzer.getTokenizer(text);
    // NOLINTEND

    while (DEBUG_VAR &&token = tokenizer.yield()) {
        REQUIRE(token.getId() != 1);
    }
}
