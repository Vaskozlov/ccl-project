#ifndef CCL_PROJECT_REPETITION_HPP
#define CCL_PROJECT_REPETITION_HPP

#include <ccl/text/text_iterator.hpp>

namespace ccl::lex::dot_item
{
    class CCL_TRIVIAL_ABI Repetition
    {
    public:
        size_t from{};
        size_t to{};

        constexpr Repetition(size_t repetition_begin, size_t repetition_end) noexcept
          : from{repetition_begin}
          , to{repetition_end}
        {}

        explicit Repetition(text::TextIterator &text_iterator);

        [[nodiscard]] consteval static auto basic() noexcept -> Repetition
        {
            return {1, 1};
        }

        [[nodiscard]] consteval static auto question() noexcept -> Repetition
        {
            return {0, 1};
        }

        [[nodiscard]] consteval static auto star() noexcept -> Repetition
        {
            return {0, max()};
        }

        [[nodiscard]] consteval static auto plus() noexcept -> Repetition
        {
            return {1, max()};
        }

        [[nodiscard]] consteval static auto max() noexcept -> size_t
        {
            return std::numeric_limits<size_t>::max();
        }

        [[nodiscard]] auto inRange(size_t value) const noexcept
        {
            return land(value >= from, value <= to);
        }

        [[nodiscard]] auto operator<=>(const Repetition &) const noexcept
            -> std::weak_ordering = default;

    private:
        [[nodiscard]] static auto
            parseNumber(text::TextIterator &text_iterator, char32_t terminator) -> size_t;

        static auto checkRangeStart(text::TextIterator &text_iterator) -> void;
        auto checkCorrectnessOfValues(text::TextIterator &text_iterator) const -> void;

        auto throwBadValues(text::TextIterator &text_iterator) const -> void;

        static auto throwRangeBeginException(text::TextIterator &text_iterator) -> void;
        static auto throwUnexpectedCharacter(text::TextIterator &text_iterator, char32_t chr)
            -> void;
    };
}// namespace ccl::lex::dot_item

#endif /* CCL_PROJECT_REPETITION_HPP */
