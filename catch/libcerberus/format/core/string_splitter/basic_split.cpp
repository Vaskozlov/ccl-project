#include <cerberus/debug/debug.hpp>
#include <cerberus/format/core/string_splitter.hpp>

using namespace cerb::fmt::core;
using namespace std::string_view_literals;

CONSTEXPR_TEST
{
    auto blocks = splitString<"">();
    ASSERT_EQ(blocks[0], ""sv);
    return {};
}
();

CONSTEXPR_TEST
{
    auto blocks = splitString<"Hello!">();
    ASSERT_EQ(blocks[0], "Hello!"sv);
    return {};
}
();

CONSTEXPR_TEST
{
    auto blocks = splitString<"Hello {}, {}!">();
    ASSERT_EQ(blocks[0], "Hello "sv);
    ASSERT_EQ(blocks[1], ", "sv);
    ASSERT_EQ(blocks[2], "!"sv);
    return {};
}
();
