#include <boost/test/unit_test.hpp>
#include <cerberus/lex/dot_item/dot_item.hpp>

using namespace cerb::lex;
using namespace cerb::text;
using namespace cerb::lex::dot_item;

BOOST_AUTO_TEST_SUITE(DotItemStringsAndCharactersCreation)

BOOST_AUTO_TEST_CASE(DotItemCharacterCreation)
{
    auto shared = AnalysisShared{};
    auto dot_item = DotItem(TextIterator{ u8R"("\'"c)" }, 0, shared);

    BOOST_ASSERT(shared.strings_and_chars.size() == 1);
    BOOST_ASSERT(shared.strings_and_chars[0].str_begin == u8R"(')");
    BOOST_ASSERT(shared.strings_and_chars[0].str_end == u8R"(')");
    BOOST_ASSERT(not shared.strings_and_chars[0].is_multiline);
    BOOST_ASSERT(shared.strings_and_chars[0].is_character);
}

BOOST_AUTO_TEST_CASE(DotItemRawStringCreation)
{
    auto shared = AnalysisShared{};
    auto dot_item = DotItem(TextIterator{ u8R"( "R\"(:)\""m )" }, 0, shared);

    BOOST_ASSERT(shared.strings_and_chars.size() == 1);
    BOOST_ASSERT(shared.strings_and_chars[0].str_begin == u8R"(R"()");
    BOOST_ASSERT(shared.strings_and_chars[0].str_end == u8")\"");
    BOOST_ASSERT(shared.strings_and_chars[0].is_multiline);
    BOOST_ASSERT(not shared.strings_and_chars[0].is_character);
}

BOOST_AUTO_TEST_CASE(DotItemTwoStringCreation)
{
    auto shared = AnalysisShared{};
    auto dot_item = DotItem(TextIterator{ u8R"("\""s "'"s)" }, 0, shared);

    BOOST_ASSERT(shared.strings_and_chars.size() == 2);

    BOOST_ASSERT(shared.strings_and_chars[0].str_begin == u8R"(")");
    BOOST_ASSERT(shared.strings_and_chars[1].str_end == u8R"(')");

    for (auto &string : shared.strings_and_chars) {
        BOOST_ASSERT(not string.is_multiline);
        BOOST_ASSERT(not string.is_character);
    }
}

BOOST_AUTO_TEST_SUITE_END()
