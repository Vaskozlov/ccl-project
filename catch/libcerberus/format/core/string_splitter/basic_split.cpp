#include <cerberus/debug/debug.hpp>
#include <cerberus/format/core/string_splitter.hpp>

using namespace cerb::fmt::core;
using namespace std::string_view_literals;

// NOLINTNEXTLINE
DEBUG_SCOPE
{
    STATIC_VARIABLE auto blocks = splitString<"">();
    STATIC_ASSERT_EQ(blocks[0], ""sv);
    return {};
}

// NOLINTNEXTLINE
DEBUG_SCOPE
{
    STATIC_VARIABLE auto blocks = splitString<"Hello!">();
    STATIC_ASSERT_EQ(blocks[0], "Hello!"sv);
    return {};
}

// NOLINTNEXTLINE
DEBUG_SCOPE
{
    STATIC_VARIABLE auto blocks = splitString<"Hello {}, {}!">();
    STATIC_ASSERT_EQ(blocks[0], "Hello "sv);
    STATIC_ASSERT_EQ(blocks[1], ", "sv);
    STATIC_ASSERT_EQ(blocks[2], "!"sv);
    return {};
}
