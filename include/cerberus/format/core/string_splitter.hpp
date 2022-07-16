#ifndef CERBERUS_PROJECT_STRING_SPLITTER_HPP
#define CERBERUS_PROJECT_STRING_SPLITTER_HPP

#include <array>
#include <cerberus/format/core/arguments_counter.hpp>
#include <cerberus/format/core/bracket_ops.hpp>
#include <cerberus/format/core/exceptions.hpp>

namespace cerb::fmt::core
{
    template<ConstString String>
    class StringSplitter
    {
    private:
        using CharT = decltype(String.zeroChar());
        using value_type = BasicStringView<CharT>;
        using iterator = typename value_type::iterator;
        using const_iterator = typename value_type::const_iterator;

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
            auto text = String.strView();

            while (text_iterator.nextRawChar() != 0) {
                processState();
            }

            constructBlock(block_begin, text.end());
        }

        constexpr auto processState() -> void
        {
            if (bracket::isOpened(text_iterator)) {
                fillBlock();
            } else if (bracket::isClosed(text_iterator)) {
                block_begin = text_iterator.getRemaining().begin() + 1;
            }
        }

        constexpr auto fillBlock() -> void
        {
            auto new_begin = text_iterator.getRemaining().begin();
            constructBlock(block_begin, new_begin);
            block_begin = new_begin;
        }

        constexpr auto constructBlock(iterator begin, iterator end) -> void
        {
            blocks[current_block++] = { begin, end };
        }

        storage_t blocks{};
        text::BasicTextIterator<CharT> text_iterator{ String.strView() };
        const_iterator block_begin{ text_iterator.getCarriage() };
        size_t current_block{};
    };

    template<ConstString String>
    CERBLIB_DECL auto splitString() -> typename StringSplitter<String>::storage_t
    {
        auto splitter = StringSplitter<String>{};
        return splitter.get();
    }
}// namespace cerb::fmt::core

#endif /* CERBERUS_PROJECT_STRING_SPLITTER_HPP */
