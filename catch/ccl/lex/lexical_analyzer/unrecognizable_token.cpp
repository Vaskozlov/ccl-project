#include <ccl/debug/debug_file.hpp>
#include <ccl/lex/tokenizer.hpp>

using namespace ccl;
using namespace lex;
using namespace text;

TEST_CASE("LexicalAnalyzerUnrecognizableTokenError", "[ContainerUnion]")
{
    auto analyzer = LexicalAnalyzer{ExceptionHandler::instance(), {{2, "[a-z]+"}}};
    auto tokenizer = analyzer.getTokenizer(R"(20)");
    auto token = tokenizer.yield();
}
