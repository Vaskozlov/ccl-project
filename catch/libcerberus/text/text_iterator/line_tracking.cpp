#include <cerberus/debug/debug.hpp>
#include <cerberus/text/text_iterator.hpp>

using namespace cerb::text;
using namespace cerb::integral_literals;
using namespace std::string_view_literals;

// NOLINTNEXTLINE
STRING_TEST
{
    auto text_iterator = TextIterator{ "Hi\nWorld!"sv };

    for (size_t i = 0; i != 3; ++i) {
        text_iterator.nextRawChar();
        ASSERT_EQ(text_iterator.getWorkingLine(), "Hi"sv);
    }

    text_iterator.nextRawChar();
    ASSERT_EQ(text_iterator.getWorkingLine(), "World!"sv);

    return {};
}
();
