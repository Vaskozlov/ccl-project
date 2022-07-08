#include <cerberus/debug/debug.hpp>
#include <cerberus/format/core/string_splitter.hpp>

using namespace cerb::fmt::core;
using namespace std::string_view_literals;

// NOLINTNEXTLINE
DEBUG_SCOPE
{
    STATIC_VARIABLE auto blocks = splitString<u"">();
    STATIC_ASSERT_EQ(blocks[0], u""sv);
    return {};
}

// NOLINTNEXTLINE
DEBUG_SCOPE
{
    STATIC_VARIABLE auto blocks = splitString<u"Hello!">();
    STATIC_ASSERT_EQ(blocks[0], u"Hello!"sv);
    return {};
}

// NOLINTNEXTLINE
DEBUG_SCOPE
{
    STATIC_VARIABLE auto blocks = splitString<u"Hello {}, {}!">();
    STATIC_ASSERT_EQ(blocks[0], u"Hello "sv);
    STATIC_ASSERT_EQ(blocks[1], u", "sv);
    STATIC_ASSERT_EQ(blocks[2], u"!"sv);
    return {};
}
