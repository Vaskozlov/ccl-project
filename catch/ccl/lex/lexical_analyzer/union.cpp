#include <ccl/debug/debug_file.hpp>
#include <ccl/lexer/tokenizer.hpp>

using namespace ccl;
using namespace lexer;
using namespace text;

TEST_CASE("LexicalAnalyzerUnion", "[ContainerUnion]")
{
    auto analyzer = LexicalAnalyzer{
        ExceptionHandler::instance(),
        {{2, R"([a-z]p+[0-9]+)"}, {3, R"([0-9]+[a-z]p*)"}},
    };

    auto tokenizer = analyzer.getTokenizer(R"(20 abz10 10abz)");

    auto token = tokenizer.yield();

    REQUIRE(token.getId() == 3);
    REQUIRE(token.getRepr() == "20");
    REQUIRE(token.getPrefixes().empty());
    REQUIRE(token.getPostfixes().empty());

    token = tokenizer.yield();

    REQUIRE(token.getId() == 2);
    REQUIRE(token.getRepr() == R"(abz10)");
    REQUIRE(token.getPrefixes().size() == 1);
    REQUIRE(token.getPostfixes().empty());
    REQUIRE(token.getPrefixes()[0] == "abz");
    REQUIRE(token.getColumn() == 4);

    token = tokenizer.yield();

    REQUIRE(token.getId() == 3);
    REQUIRE(token.getRepr() == "10abz");
    REQUIRE(token.getPrefixes().empty());
    REQUIRE(token.getPostfixes().size() == 1);
    REQUIRE(token.getPostfixes()[0] == "abz");
    REQUIRE(token.getColumn() == 10);
}
