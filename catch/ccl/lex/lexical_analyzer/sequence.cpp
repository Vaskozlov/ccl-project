#include <ccl/debug/debug_file.hpp>
#include <ccl/lex/tokenizer.hpp>

using namespace ccl;
using namespace lex;
using namespace text;

BOOST_AUTO_TEST_CASE(LexicalAnalyzerSequence, *utf::depends_on("ContainerSequence"))
{
    auto analyzer =
        LexicalAnalyzer(ExceptionHandler::instance(), {{2, R"("abz"p"10")"}, {3, R"("10""abz"p)"}});

    auto tokenizer = analyzer.getTokenizer(R"(abz10 10abz)");
    const auto &future_token = tokenizer.yieldFutureToken();
    const auto &token = tokenizer.yield();

    BOOST_ASSERT(token.getId() == future_token.getId());

    BOOST_ASSERT(token.getId() == 2);
    BOOST_ASSERT(token.getRepr() == R"(abz10)");
    BOOST_ASSERT(token.getPrefixes().size() == 1);
    BOOST_ASSERT(token.getPrefixes()[0] == "abz");

    const auto &token_2 = tokenizer.yield();

    BOOST_ASSERT(token_2.getId() == 3);
    BOOST_ASSERT(token_2.getRepr() == R"(10abz)");
    BOOST_ASSERT(token_2.getPostfixes().size() == 1);
    BOOST_ASSERT(token_2.getPostfixes()[0] == "abz");
}
