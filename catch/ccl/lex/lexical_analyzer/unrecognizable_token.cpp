#include <ccl/debug/debug_file.hpp>
import ccl.lexer;
import ccl.debug;

TEST_CASE("LexicalAnalyzerUnrecognizableTokenError", "[ContainerUnion]")
{
    using namespace ccl;

    auto analyzer = lexer::LexicalAnalyzer{
        handler::ExceptionHandler::instance(), {{debug::FirstUsableToken, "[a-z]+"}}};
    auto tokenizer = analyzer.getTokenizer(R"(20)");
    const lexer::Token &token = tokenizer.yield();

    REQUIRE(token.getRepr() == "20");
    REQUIRE(token.getColumn() == 0);
    REQUIRE(token.getLine() == 1);
}
