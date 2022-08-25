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
        using value_type = u8string_view;
        using iterator = typename value_type::iterator;
        using const_iterator = typename value_type::const_iterator;

        static constexpr size_t blocks_num = countArgs(String) + 1;

    public:
        using storage_t = std::array<value_type, blocks_num>;

        CERBLIB_DECL auto get() const noexcept -> const storage_t &
        {
            return blocks;
        }

        auto operator=(StringSplitter &&) -> void = delete;
        auto operator=(const StringSplitter &) -> void = delete;

        constexpr StringSplitter()
        {
            fill();

            if (not isEveryBlockFilled()) {
                throw StringSplitterException("Not every block of formatter has been filled");
            }
        }

        StringSplitter(StringSplitter &&) = delete;
        StringSplitter(const StringSplitter &) = delete;

        ~StringSplitter() = default;

    private:
        CERBLIB_DECL auto isEveryBlockFilled() const noexcept -> bool
        {
            return current_block == blocks_num;
        }

        constexpr auto fill() -> void
        {
            auto text = static_cast<u8string_view>(String);

            while (text_iterator.next() != 0) {
                processState();
            }

            constructBlock(block_begin, text.end());
        }

        constexpr auto processState() -> void
        {
            if (bracket::isOpened(text_iterator)) {
                fillBlock();
            } else if (bracket::isClosed(text_iterator)) {
                block_begin = text_iterator.getRemainingAsCarriage();
            }
        }

        constexpr auto fillBlock() -> void
        {
            const auto *new_begin = text_iterator.getCarriage();
            constructBlock(block_begin, new_begin);
            block_begin = new_begin;
        }

        constexpr auto constructBlock(iterator begin, iterator end) -> void
        {
            blocks[current_block++] = { begin, end };
        }

        storage_t blocks{};
        text::BasicTextIterator text_iterator{ String };
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
