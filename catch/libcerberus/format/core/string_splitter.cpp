#include <cerberus/debug/debug.hpp>
#include <cerberus/format/core/string_splitter.hpp>

// NOLINTBEGIN

using namespace std::string_view_literals;

template<cerb::TemplateString String>
using StringSplitter = cerb::fmt::StringSplitter<String>;

template struct cerb::fmt::StringSplitter<"Hello!">;

DEBUG_SCOPE
{
    StringSplitter<"Hello!"> splitter;

    auto &blocks = splitter.getBlocks();

    ASSERT_EQ(blocks.size(), 1U);
    ASSERT_EQ(blocks[0], "Hello!"sv);

    return {};
}

DEBUG_SCOPE
{
    StringSplitter<"Hello!{}"> splitter;

    auto &blocks = splitter.getBlocks();

    ASSERT_EQ(blocks.size(), 2U);
    ASSERT_EQ(blocks[0], "Hello!"sv);
    ASSERT_EQ(blocks[1], ""sv);

    return {};
}

DEBUG_SCOPE
{
    StringSplitter<"Hello {}, {}!"> splitter;

    auto &blocks = splitter.getBlocks();

    ASSERT_EQ(blocks.size(), 3U);
    ASSERT_EQ(blocks[0], "Hello "sv);
    ASSERT_EQ(blocks[1], ", "sv);
    ASSERT_EQ(blocks[2], "!"sv);

    return {};
}


DEBUG_SCOPE
{
    StringSplitter<"Hello {}, {}!"> splitter;

    auto &blocks = splitter.getBlocks();

    ASSERT_EQ(blocks.size(), 3U);
    ASSERT_EQ(blocks[0], "Hello "sv);
    ASSERT_EQ(blocks[1], ", "sv);
    ASSERT_EQ(blocks[2], "!"sv);

    return {};
}

// NOLINTEND
