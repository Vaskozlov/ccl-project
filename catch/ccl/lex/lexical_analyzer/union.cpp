#include <ccl/debug/debug.hpp>
#include <ccl/lexer/tokenizer.hpp>

TEST_CASE("LexicalAnalyzerUnion", "[ContainerUnion]")
{
    using namespace ccl;

    auto analyzer = lexer::LexicalAnalyzer{
        ExceptionHandler::instance(),
        {
            {debug::FirstUsableToken, R"([a-z]p+[0-9]+)"},
            {debug::FirstUsableToken + 1, R"([0-9]+[a-z]p*)"},
        },
    };

    auto tokenizer = analyzer.getTokenizer(R"(20 abz10 10abz)");
    auto token = tokenizer.yield();

    REQUIRE(token.getId() == debug::FirstUsableToken + 1);
    REQUIRE(token.getRepr() == "20");
    REQUIRE(token.getPrefixes().empty());
    REQUIRE(token.getPostfixes().empty());

    token = tokenizer.yield();

    REQUIRE(token.getId() == debug::FirstUsableToken);
    REQUIRE(token.getRepr() == R"(abz10)");
    REQUIRE(token.getPrefixes().size() == 1);
    REQUIRE(token.getPostfixes().empty());
    REQUIRE(token.getPrefixes().at(0) == "abz");
    REQUIRE(token.getColumn() == 4);

    token = tokenizer.yield();

    REQUIRE(token.getId() == debug::FirstUsableToken + 1);
    REQUIRE(token.getRepr() == "10abz");
    REQUIRE(token.getPrefixes().empty());
    REQUIRE(token.getPostfixes().size() == 1);
    REQUIRE(token.getPostfixes().at(0) == "abz");
    REQUIRE(token.getColumn() == 10);
}
