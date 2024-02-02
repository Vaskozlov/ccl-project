#include <ccl/debug/debug_file.hpp>
#include <ccl/lexer/tokenizer.hpp>

using namespace ccl;
using namespace debug;
using namespace lexer;
using namespace text;

TEST_CASE("LexicalAnalyzerSequence", "[ContainerSequence]")
{
    auto analyzer = LexicalAnalyzer(
        ExceptionHandler::instance(),
        {{FirstUsableToken, R"("abz"p"10")"}, {FirstUsableToken + 1, R"("10""abz"p)"}});

    auto tokenizer = analyzer.getTokenizer(R"(abz10 10abz)");
    DEBUG_VAR &&future_token = tokenizer.yieldFutureToken();
    DEBUG_VAR &&token = tokenizer.yield();

    REQUIRE(token.getId() == future_token.getId());

    REQUIRE(token.getId() == FirstUsableToken);
    REQUIRE(token.getRepr() == R"(abz10)");
    REQUIRE(token.getPrefixes().size() == 1);
    REQUIRE(token.getPrefixes()[0] == "abz");

    DEBUG_VAR &&token_2 = tokenizer.yield();

    REQUIRE(token_2.getId() == FirstUsableToken + 1);
    REQUIRE(token_2.getRepr() == R"(10abz)");
    REQUIRE(token_2.getPostfixes().size() == 1);
    REQUIRE(token_2.getPostfixes()[0] == "abz");
}
