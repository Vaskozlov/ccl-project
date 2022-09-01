#include <ccl/debug/debug_file.hpp>
#include <ccl/lex/lexical_analyzer.hpp>

using namespace ccl;
using namespace lex;
using namespace text;

BOOST_AUTO_TEST_SUITE(
    LexicalAnalyzerCharacter,
    *utf::depends_on("ContainerStringsAndCharactersCreation"))

BOOST_AUTO_TEST_CASE(LexicalAnalyzerCharacterMatching)
{
    auto analyzer = LexicalAnalyzer{ ExceptionHandler::instance(), { { 2, R"("\'"c)" } } };
    auto tokenizer = analyzer.getTokenizer(R"('A')");

    DEBUG_VAR token = tokenizer.yield();

    BOOST_ASSERT(token.getRepr() == "'A'");
    BOOST_ASSERT(token.getValue() == "A");
    BOOST_ASSERT(token.getPrefixes().empty());
    BOOST_ASSERT(token.getPostfixes().empty());
}

BOOST_AUTO_TEST_CASE(LexicalAnalyzerUtfCharacterMatching)
{
    auto analyzer = LexicalAnalyzer{ ExceptionHandler::instance(), { { 2, R"("\'"c)" } } };
    auto tokenizer = analyzer.getTokenizer(R"('\uFFFF')");

    DEBUG_VAR token = tokenizer.yield();

    BOOST_ASSERT(token.getRepr() == "'\\uFFFF'");
    BOOST_ASSERT(token.getValue() == "\uFFFF");
    BOOST_ASSERT(token.getPrefixes().empty());
    BOOST_ASSERT(token.getPostfixes().empty());
}

BOOST_AUTO_TEST_CASE(LexicalAnalyzerEmptyCharacter)
{
    auto analyzer = LexicalAnalyzer{ ExceptionHandler::instance(), { { 2, R"("\'"c)" } } };
    auto tokenizer = analyzer.getTokenizer(R"('')");

    DEBUG_VAR token = tokenizer.yield();

    BOOST_ASSERT(token.getId() == ReservedTokenType::BAD_TOKEN);
}

BOOST_AUTO_TEST_CASE(LexicalAnalyzerTwoCharacters)
{
    auto analyzer = LexicalAnalyzer{ ExceptionHandler::instance(), { { 2, R"("\'"c)" } } };
    auto tokenizer = analyzer.getTokenizer(R"('AA')");

    DEBUG_VAR token = tokenizer.yield();

    BOOST_ASSERT(token.getId() == ReservedTokenType::BAD_TOKEN);
}

BOOST_AUTO_TEST_SUITE_END()
