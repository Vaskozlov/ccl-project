#include <cerberus/debug/debug_file.hpp>
#include <cerberus/lex/lexical_analyzer.hpp>

using namespace cerb::lex;
using namespace cerb::text;

BOOST_AUTO_TEST_SUITE(LexicalAnalyzerCharacter)

BOOST_AUTO_TEST_CASE(LexicalAnalyzerCharacterMatching)
{
    auto text = TextIterator{ u8R"('A')" };
    auto analyzer = LexicalAnalyzer{ { 1, u8R"("\'"c)" } };
    auto token = analyzer.yield(text);

    BOOST_ASSERT(token.getId() == 1);
    BOOST_ASSERT(token.getRepr() == u8"'A'");
    BOOST_ASSERT(token.getValue() == u8"A");
    BOOST_ASSERT(token.getPrefixes().empty());
    BOOST_ASSERT(token.getPostfixes().empty());
}

BOOST_AUTO_TEST_CASE(LexicalAnalyzerUtfCharacterMatching)
{
    auto text = TextIterator{ u8R"('\uFFFF')" };
    auto analyzer = LexicalAnalyzer{ { 1, u8R"("\'"c)" } };
    auto token = analyzer.yield(text);

    BOOST_ASSERT(token.getId() == 1);
    BOOST_ASSERT(token.getRepr() == u8"'\\uFFFF'");
    BOOST_ASSERT(token.getValue() == u8"\uFFFF");
    BOOST_ASSERT(token.getPrefixes().empty());
    BOOST_ASSERT(token.getPostfixes().empty());
}

BOOST_AUTO_TEST_CASE(LexicalAnalyzerEmptyCharacter)
{
    auto text = TextIterator{ u8R"('')" };
    auto analyzer = LexicalAnalyzer{ { 1, u8R"("\'"c)" } };

    BOOST_CHECK_EXCEPTION(
        analyzer.yield(text), LexicalAnalysisException,
        [](const LexicalAnalysisException &exception) {
            return exception.getMessage() == u8"empty character definition";
        });
}

BOOST_AUTO_TEST_CASE(LexicalAnalyzerTwoCharacters)
{
    auto text = TextIterator{ u8R"('AA')" };
    auto analyzer = LexicalAnalyzer{ { 1, u8R"("\'"c)" } };

    BOOST_CHECK_EXCEPTION(
        analyzer.yield(text), LexicalAnalysisException,
        [](const LexicalAnalysisException &exception) {
            return exception.getMessage() == u8"character definition must be a single character";
        });
}

BOOST_AUTO_TEST_SUITE_END()
