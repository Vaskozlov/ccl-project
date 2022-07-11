#include <cerberus/debug/debug.hpp>
#include <cerberus/text/text_iterator.hpp>

using namespace cerb::text;
using namespace cerb::integral_literals;
using namespace std::string_view_literals;

STRING_TEST
{
    auto text_iterator = TextIterator{ ""sv };

    ASSERT_EQ(text_iterator.getLine(), 1_ZU);
    ASSERT_EQ(text_iterator.getColumn(), 1_ZU);

    text_iterator.nextRawChar();

    ASSERT_EQ(text_iterator.getLine(), 1_ZU);
    ASSERT_EQ(text_iterator.getColumn(), 1_ZU);

    text_iterator.nextRawChar();

    ASSERT_EQ(text_iterator.getLine(), 1_ZU);
    ASSERT_EQ(text_iterator.getColumn(), 1_ZU);

    return {};
}
();

STRING_TEST
{
    auto text_iterator = TextIterator{ "Hi\nWorld!"sv };

    ASSERT_EQ(text_iterator.getLine(), 1_ZU);
    ASSERT_EQ(text_iterator.getColumn(), 1_ZU);

    text_iterator.nextRawChar();

    ASSERT_EQ(text_iterator.getLine(), 1_ZU);
    ASSERT_EQ(text_iterator.getColumn(), 2_ZU);

    text_iterator.nextRawChar();

    ASSERT_EQ(text_iterator.getLine(), 1_ZU);
    ASSERT_EQ(text_iterator.getColumn(), 3_ZU);

    text_iterator.nextRawChar();

    ASSERT_EQ(text_iterator.getLine(), 2_ZU);
    ASSERT_EQ(text_iterator.getColumn(), 1_ZU);

    return {};
}
();
