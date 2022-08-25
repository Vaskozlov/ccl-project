#ifndef CCL_PROJECT_ARGUMENTS_COUNTER_HPP
#define CCL_PROJECT_ARGUMENTS_COUNTER_HPP

#include <ccl/const_string.hpp>
#include <ccl/format/core/bracket_ops.hpp>
#include <ccl/format/core/exceptions.hpp>

namespace ccl::fmt::core
{
    class ArgumentsCounter
    {
    public:
        CCL_DECL auto get() const noexcept -> size_t
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
            while (text_iterator.next() != 0) {
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
                text_iterator.next();
            }
        }

        CCL_DECL auto isUnbalancedBraces() const noexcept -> bool
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

}// namespace ccl::fmt::core

#endif /* CCL_PROJECT_ARGUMENTS_COUNTER_HPP */
