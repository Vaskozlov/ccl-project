#include <ccl/debug/debug.hpp>
#include <ccl/lexer/tokenizer.hpp>

TEST_CASE("LexicalAnalyzerUnrecognizableTokenError", "[ContainerUnion]")
{
    using namespace ccl;

    auto analyzer =
        lexer::LexicalAnalyzer{ExceptionHandler::instance(), {{debug::FirstUsableToken, "[a-z]+"}}};
    auto tokenizer = analyzer.getTokenizer(R"(20)");
    const lexer::Token &token = tokenizer.yield();

    REQUIRE(token.getRepr() == "20");
    REQUIRE(token.getColumn() == 0);
    REQUIRE(token.getLine() == 1);
}
