#include <ccl/debug/debug_file.hpp>
#include <ccl/lex/tokenizer.hpp>

using namespace ccl;
using namespace lex;
using namespace text;

TEST_CASE("LexicalAnalyzerSequence", "[ContainerSequence]")
{
    auto analyzer =
        LexicalAnalyzer(ExceptionHandler::instance(), {{2, R"("abz"p"10")"}, {3, R"("10""abz"p)"}});

    auto tokenizer = analyzer.getTokenizer(R"(abz10 10abz)");
    DEBUG_VAR &&future_token = tokenizer.yieldFutureToken();
    DEBUG_VAR &&token = tokenizer.yield();

    REQUIRE(token.getId() == future_token.getId());

    REQUIRE(token.getId() == 2);
    REQUIRE(token.getRepr() == R"(abz10)");
    REQUIRE(token.getPrefixes().size() == 1);
    REQUIRE(token.getPrefixes()[0] == "abz");

    DEBUG_VAR &&token_2 = tokenizer.yield();

    REQUIRE(token_2.getId() == 3);
    REQUIRE(token_2.getRepr() == R"(10abz)");
    REQUIRE(token_2.getPostfixes().size() == 1);
    REQUIRE(token_2.getPostfixes()[0] == "abz");
}
