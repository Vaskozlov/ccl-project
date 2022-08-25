#include <cerberus/debug/debug_file.hpp>
#include <cerberus/lex/lexical_analyzer.hpp>

using namespace cerb::lex;
using namespace cerb::text;

BOOST_AUTO_TEST_SUITE(
    LexicalAnalyzerCharacter,
    *utf::depends_on("DotItemStringsAndCharactersCreation"))

BOOST_AUTO_TEST_CASE(LexicalAnalyzerCharacterMatching)
{
    auto analyzer = LexicalAnalyzer{ { 1, u8R"("\'"c)" } };
    auto tokenizer = analyzer.getTokenizer(u8R"('A')");

    auto token = tokenizer.yield();

    BOOST_ASSERT(token.getId() == 1);
    BOOST_ASSERT(token.getRepr() == u8"'A'");
    BOOST_ASSERT(token.getValue() == u8"A");
    BOOST_ASSERT(token.getPrefixes().empty());
    BOOST_ASSERT(token.getPostfixes().empty());
}

BOOST_AUTO_TEST_CASE(LexicalAnalyzerUtfCharacterMatching)
{
    auto analyzer = LexicalAnalyzer{ { 1, u8R"("\'"c)" } };
    auto tokenizer = analyzer.getTokenizer(u8R"('\uFFFF')");

    auto token = tokenizer.yield();

    BOOST_ASSERT(token.getId() == 1);
    BOOST_ASSERT(token.getRepr() == u8"'\\uFFFF'");
    BOOST_ASSERT(token.getValue() == u8"\uFFFF");
    BOOST_ASSERT(token.getPrefixes().empty());
    BOOST_ASSERT(token.getPostfixes().empty());
}

BOOST_AUTO_TEST_CASE(LexicalAnalyzerEmptyCharacter)
{
    auto analyzer = LexicalAnalyzer{ { 1, u8R"("\'"c)" } };
    auto tokenizer = analyzer.getTokenizer(u8R"('')");

    auto token = tokenizer.yield();

    BOOST_ASSERT(token.getId() == ReservedTokenType::BAD_TOKEN);
    BOOST_ASSERT(
        analyzer.getExceptionAccumulator().getErrors()[0]->getMessage() ==
        u8"empty character definition");
}

BOOST_AUTO_TEST_CASE(LexicalAnalyzerTwoCharacters)
{
    auto analyzer = LexicalAnalyzer{ { 1, u8R"("\'"c)" } };
    auto tokenizer = analyzer.getTokenizer(u8R"('AA')");

    auto token = tokenizer.yield();

    BOOST_ASSERT(token.getId() == ReservedTokenType::BAD_TOKEN);

    BOOST_ASSERT(
        analyzer.getExceptionAccumulator().getErrors()[0]->getMessage() ==
        u8"character definition must be a single character");
}

BOOST_AUTO_TEST_SUITE_END()
