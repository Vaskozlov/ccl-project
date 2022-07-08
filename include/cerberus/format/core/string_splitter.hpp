#ifndef CERBERUS_PROJECT_STRING_SPLITTER_HPP
#define CERBERUS_PROJECT_STRING_SPLITTER_HPP

#include <array>
#include <cerberus/format/core/arguments_counter.hpp>
#include <cerberus/format/core/bracket_ops.hpp>
#include <cerberus/format/core/exceptions.hpp>

namespace cerb::fmt::core
{
    template<TemplateString String>
    class StringSplitter
    {
    private:
        using CharT = decltype(String.zeroChar());
        using value_type = std::basic_string_view<CharT>;
        using iterator = typename std::basic_string_view<CharT>::iterator;
        static constexpr size_t blocks_num = core::countArgs(String.strView()) + 1;

    public:
        using storage_t = std::array<value_type, blocks_num>;

        CERBLIB_DECL auto get() const -> const std::array<value_type, blocks_num> &
        {
            return blocks;
        }

        auto operator=(StringSplitter &&) -> StringSplitter & = delete;
        auto operator=(const StringSplitter &) -> StringSplitter & = delete;

        constexpr StringSplitter()
        {
            fill();

            if (not isEveryBlockFilled()) {
                throw StringSplitterException{ "Not every block of formatter has been filled" };
            }
        }

        StringSplitter(StringSplitter &&) = delete;
        StringSplitter(const StringSplitter &) = delete;

        ~StringSplitter() = default;

    private:
        CERBLIB_DECL auto isEveryBlockFilled() const -> bool
        {
            return current_block == blocks_num;
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

        storage_t blocks{};
        std::basic_string_view<CharT> text{ String.strView() };
        text::BasicTextIterator<CharT> text_iterator{ text };
        size_t offset{ text_iterator.getOffset() };
        size_t current_block{};
    };

    template<TemplateString String>
    CERBLIB_DECL auto splitString() -> typename StringSplitter<String>::storage_t
    {
        auto splitter = StringSplitter<String>{};
        return splitter.get();
    }
}// namespace cerb::fmt::core

#endif /* CERBERUS_PROJECT_STRING_SPLITTER_HPP */
