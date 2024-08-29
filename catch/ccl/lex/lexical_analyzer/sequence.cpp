#include <ccl/debug/debug.hpp>
#include <ccl/lexer/tokenizer.hpp>

TEST_CASE("LexicalAnalyzerSequence", "[ContainerSequence]")
{
    using namespace ccl;

    const auto analyzer = lexer::LexicalAnalyzer(
        ExceptionHandler::instance(),
        {
            {debug::FirstUsableToken, R"("abz"e"10")"},
            {debug::FirstUsableToken + 1, R"("10""abz"e)"},
        });

    auto tokenizer = analyzer.getTokenizer(R"(abz10 10abz)");
    DEBUG_VAR future_token = tokenizer.yieldFutureToken();
    DEBUG_VAR token = tokenizer.yield();

    REQUIRE(token.getId() == future_token.getId());

    REQUIRE(token.getId() == debug::FirstUsableToken);
    REQUIRE(token.getRepr() == R"(abz10)");
    REQUIRE(token.getExtractedParts().size() == 1);
    REQUIRE(token.getExtractedParts().at(0) == "abz");

    DEBUG_VAR token_2 = tokenizer.yield();

    REQUIRE(token_2.getId() == debug::FirstUsableToken + 1);
    REQUIRE(token_2.getRepr() == R"(10abz)");
    REQUIRE(token_2.getExtractedParts().size() == 1);
    REQUIRE(token_2.getExtractedParts().at(0) == "abz");
}
