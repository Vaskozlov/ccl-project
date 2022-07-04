#ifndef CERBERUS_PROJECT_BLOCK_FILLER_HPP
#define CERBERUS_PROJECT_BLOCK_FILLER_HPP

#include <cerberus/format/core/bracket_ops.hpp>
#include <cerberus/format/core/exceptions.hpp>

namespace cerb::fmt::core
{
    template<size_t N, CharacterLiteral CharT>
    class BlockFiller
    {
    private:
        using value_type = std::basic_string_view<CharT>;
        using iterator = typename std::basic_string_view<CharT>::iterator;

    public:
        CERBLIB_DECL auto get() const -> const std::array<value_type, N> &
        {
            return blocks;
        }

        BlockFiller() = default;

        constexpr explicit BlockFiller(const std::basic_string_view<CharT> &text_)
          : text_iterator{ text_ }
        {
            fill();

            if (not isEveryBlockFilled()) {
                throw StringSplitterException{ "Not every block of formatter has been filled" };
            }
        }

    private:
        CERBLIB_DECL auto isEveryBlockFilled() const -> bool
        {
            return current_block == N;
        }

        constexpr auto fill() -> void
        {
            while (text_iterator.nextRawChar() != 0) {
                processState();
            }

            constructBlock(text.begin() + offset, text.end());
        }

        constexpr auto processState() -> void
        {
            if (bracket::isOpened(text_iterator)) {
                fillBlock();
            } else if (bracket::isClosed(text_iterator)) {
                offset = text_iterator.getOffset() + 1;
            }
        }

        constexpr auto fillBlock() -> void
        {
            auto new_offset = text_iterator.getOffset();
            constructBlock(text.begin() + offset, text.begin() + new_offset);
            offset = new_offset;
        }

        constexpr auto constructBlock(iterator begin, iterator end) -> void
        {
            blocks[current_block++] = { begin, end };
        }

        std::array<value_type, N> blocks{};
        text::BasicTextIterator<CharT> text_iterator{};
        std::basic_string_view<CharT> text{ text_iterator.getText() };
        size_t offset{ text_iterator.getOffset() };
        size_t current_block{};
    };

    template<size_t N, CharacterLiteral CharT>
    CERBLIB_DECL auto fillBlocks(const std::basic_string_view<CharT> &text)
        -> std::array<std::basic_string_view<CharT>, N>
    {
        BlockFiller<N, CharT> filler{ text };
        return filler.get();
    }
}// namespace cerb::fmt::core

#endif /* CERBERUS_PROJECT_BLOCK_FILLER_HPP */
