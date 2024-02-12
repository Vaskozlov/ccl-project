#include <ccl/debug/debug_file.hpp>
#include <ccl/lexer/tokenizer.hpp>

TEST_CASE("LexicalAnalyzerSequence", "[ContainerSequence]")
{
    using namespace ccl;

    auto analyzer = lexer::LexicalAnalyzer(
        ExceptionHandler::instance(),
        {{debug::FirstUsableToken, R"("abz"p"10")"},
         {debug::FirstUsableToken + 1, R"("10""abz"p)"}});

    auto tokenizer = analyzer.getTokenizer(R"(abz10 10abz)");
    DEBUG_VAR future_token = tokenizer.yieldFutureToken();
    DEBUG_VAR token = tokenizer.yield();

    REQUIRE(token.getId() == future_token.getId());

    REQUIRE(token.getId() == debug::FirstUsableToken);
    REQUIRE(token.getRepr() == R"(abz10)");
    REQUIRE(token.getPrefixes().size() == 1);
    REQUIRE(token.getPrefixes()[0] == "abz");

    DEBUG_VAR token_2 = tokenizer.yield();

    REQUIRE(token_2.getId() == debug::FirstUsableToken + 1);
    REQUIRE(token_2.getRepr() == R"(10abz)");
    REQUIRE(token_2.getPostfixes().size() == 1);
    REQUIRE(token_2.getPostfixes()[0] == "abz");
}
