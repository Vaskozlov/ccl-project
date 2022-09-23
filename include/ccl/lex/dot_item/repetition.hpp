#ifndef CCL_PROJECT_REPETITION_HPP
#define CCL_PROJECT_REPETITION_HPP

#include <ccl/text/text_iterator.hpp>

namespace ccl::lex::dot_item
{
    struct CCL_TRIVIAL_ABI Repetition
    {
        constexpr Repetition(size_t from_, size_t to_) noexcept : from(from_), to(to_)
        {}

        explicit Repetition(text::TextIterator &text_iterator);

        consteval static auto basic() noexcept -> Repetition
        {
            return { 1, 1 };
        }

        consteval static auto question() noexcept -> Repetition
        {
            return { 0, 1 };
        }

        consteval static auto star() noexcept -> Repetition
        {
            return { 0, max() };
        }

        consteval static auto plus() noexcept -> Repetition
        {
            return { 1, max() };
        }

        consteval static auto max() noexcept -> size_t
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
        static auto parseNumber(text::TextIterator &text_iterator, char32_t terminator) -> size_t;

        static auto checkRangeStart(text::TextIterator &text_iterator) -> void;
        auto checkCorrectnessOfValues(text::TextIterator &text_iterator) const -> void;

        auto throwBadValues(text::TextIterator &text_iterator) const -> void;

        static auto throwRangeBeginException(text::TextIterator &text_iterator) -> void;
        static auto throwUnexpectedCharacter(text::TextIterator &text_iterator, char32_t chr)
            -> void;

    public:
        size_t from{};
        size_t to{};
    };
}// namespace ccl::lex::dot_item

#endif /* CCL_PROJECT_REPETITION_HPP */