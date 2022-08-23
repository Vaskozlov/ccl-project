#ifndef CERBERUS_PROJECT_ARGUMENTS_COUNTER_HPP
#define CERBERUS_PROJECT_ARGUMENTS_COUNTER_HPP

#include <cerberus/const_string.hpp>
#include <cerberus/format/core/bracket_ops.hpp>
#include <cerberus/format/core/exceptions.hpp>

namespace cerb::fmt::core
{
    class ArgumentsCounter
    {
    public:
        CERBLIB_DECL auto get() const noexcept -> size_t
        {
            return arguments_counter;
        }

        constexpr ArgumentsCounter() noexcept = default;

        constexpr explicit ArgumentsCounter(const u8string_view &text) : text_iterator{ text }
        {
            count();

            if (isUnbalancedBraces()) {
                throw StringSplitterException{ "Unbalanced braces" };
            }
        }

    private:
        constexpr auto count() -> void
        {
            while (text_iterator.nextRawChar() != 0) {
                processState();
            }
        }

        constexpr auto processState() -> void
        {
            if (bracket::isOpened(text_iterator)) {
                ++opened_brackets_counter;
                ++arguments_counter;
            } else if (bracket::isClosed(text_iterator)) {
                --opened_brackets_counter;
            } else if (bracket::needToSkip(text_iterator)) {
                text_iterator.nextRawChar();
            }
        }

        CERBLIB_DECL auto isUnbalancedBraces() const noexcept -> bool
        {
            return opened_brackets_counter != 0;
        }

        text::BasicTextIterator text_iterator{};
        size_t arguments_counter{};
        size_t opened_brackets_counter{};
    };

    constexpr auto countArgs(const u8string_view &text) -> size_t
    {
        return ArgumentsCounter{ text }.get();
    }

}// namespace cerb::fmt::core

#endif /* CERBERUS_PROJECT_ARGUMENTS_COUNTER_HPP */
