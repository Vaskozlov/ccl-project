#include <cerberus/debug/debug.hpp>
#include <cerberus/format/core/block_filler.hpp>

// NOLINTBEGIN

using namespace std::string_view_literals;

template<size_t N, cerb::CharacterLiteral CharT>
using BlockFiller = cerb::fmt::core::BlockFiller<N, CharT>;

template class cerb::fmt::core::BlockFiller<3, char>;

DEBUG_SCOPE
{
    BlockFiller<3, char> filler{ "Hello {}, {}!" };
    auto &blocks = filler.get();

    ASSERT_EQ(blocks[0], "Hello "sv);
    ASSERT_EQ(blocks[1], ", "sv);
    ASSERT_EQ(blocks[2], "!"sv);

    return {};
}

// NOLINTEND
