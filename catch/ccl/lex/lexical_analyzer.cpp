#include <ccl/debug/debug.hpp>
#include <ccl/handler/cmd.hpp>
#include <ccl/lexer/tokenizer.hpp>

TEST_CASE("LexTest", "[LexicalAnalyzer]")
{
    using namespace ccl;
    using namespace isl::string_view_literals;

    auto text =
        R"(
        10.0f;
        auto i = 10;
        i+10; // test
        auto _ = "Hello, World!" + "test\"")"_sv;

    auto token_id = lexer::ReservedTokenMaxValue + 1;
    const auto analyzer = lexer::LexicalAnalyzer(
        handler::Cmd::instance(),
        {
            {token_id++, R"( [0-9]+"."[0-9]* ( [a-zA-Z_]+ [a-zA-Z0-9_]* )?e )"},
            {token_id++, "[a-zA-Z_]+"},
            {token_id++, "[0-9]+"},
            {token_id++, R"(! "+")"},
            {token_id++, R"(! ";")"},
            {token_id++, R"(! "=")"},
            {token_id++, R"(! "\"" ("\\\"" | ["]^)* "\"" )"},
            {token_id++, R"(! "\n")"},
            {token_id, R"(! "//"[\n]^*)"},
        });

    auto tokenizer = analyzer.getTokenizer(text);

    while (DEBUG_VAR token = tokenizer.yield()) {
        REQUIRE(token.getId() != 1);
    }
}
