#include <cerberus/debug/debug.hpp>
#include <cerberus/format/core/string_splitter.hpp>

using namespace cerb::fmt::core;
using namespace std::string_view_literals;

CONSTEXPR_TEST
{
    auto blocks = splitString<u"">();
    ASSERT_EQ(blocks[0], u""sv);
    return {};
}
();

CONSTEXPR_TEST
{
    auto blocks = splitString<u"Hello!">();
    ASSERT_EQ(blocks[0], u"Hello!"sv);
    return {};
}
();

CONSTEXPR_TEST
{
    auto blocks = splitString<u"Hello {}, {}!">();
    ASSERT_EQ(blocks[0], u"Hello "sv);
    ASSERT_EQ(blocks[1], u", "sv);
    ASSERT_EQ(blocks[2], u"!"sv);
    return {};
}
();
