#include <ccl/debug/debug.hpp>
#include <ccl/lexer/tokenizer.hpp>

TEST_CASE("LexicalAnalyzerUnion", "[ContainerUnion]")
{
    using namespace ccl;

    auto analyzer = lexer::LexicalAnalyzer{
        ExceptionHandler::instance(),
        {
            {debug::FirstUsableToken, R"([a-z]e+[0-9]+)"},
            {debug::FirstUsableToken + 1, R"([0-9]+[a-z]e*)"},
        },
    };

    auto tokenizer = analyzer.getTokenizer(R"(20 abz10 10abz)");
    auto token = tokenizer.yield();

    REQUIRE(token.getId() == debug::FirstUsableToken + 1);
    REQUIRE(token.getRepr() == "20");
    REQUIRE(token.getExtractedParts().size() == 1);

    token = tokenizer.yield();

    REQUIRE(token.getId() == debug::FirstUsableToken);
    REQUIRE(token.getRepr() == R"(abz10)");
    REQUIRE(token.getExtractedParts().size() == 1);
    REQUIRE(token.getExtractedParts().front() == "abz");
    REQUIRE(token.getColumn() == 4);

    token = tokenizer.yield();

    REQUIRE(token.getId() == debug::FirstUsableToken + 1);
    REQUIRE(token.getRepr() == "10abz");
    REQUIRE(token.getExtractedParts().size() == 1);
    REQUIRE(token.getExtractedParts().front() == "abz");
    REQUIRE(token.getColumn() == 10);
}
