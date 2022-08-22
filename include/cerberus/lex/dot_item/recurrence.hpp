#ifndef CERBERUS_PROJECT_RECURRENCE_HPP
#define CERBERUS_PROJECT_RECURRENCE_HPP

#include <cerberus/text/text_iterator.hpp>

namespace cerb::lex::dot_item
{
    CERBLIB_EXCEPTION(RecurrenceException, text::TextIteratorException);

    struct CERBLIB_TRIVIAL_ABI Recurrence
    {
        constexpr Recurrence(size_t from_, size_t to_) noexcept : from(from_), to(to_)
        {}

        explicit Recurrence(text::TextIterator &text_iterator);

        consteval static auto basic() noexcept -> Recurrence
        {
            return { 1, 1 };
        }

        consteval static auto question() noexcept -> Recurrence
        {
            return { 0, 1 };
        }

        consteval static auto star() noexcept -> Recurrence
        {
            return { 0, std::numeric_limits<size_t>::max() };
        }

        consteval static auto plus() noexcept -> Recurrence
        {
            return { 1, std::numeric_limits<size_t>::max() };
        }

        [[nodiscard]] auto inRange(size_t value) const noexcept
        {
            return land(value >= from, value <= to);
        }

        [[nodiscard]] auto operator<=>(const Recurrence &) const noexcept
            -> std::weak_ordering = default;

    private:
        static auto parseNumber(text::TextIterator &text_iterator, char32_t terminator) -> size_t;

        static auto checkRangeStart(text::TextIterator &text_iterator) -> void;
        auto checkCorrectnessOfValues(text::TextIterator &text_iterator) const -> void;

        auto throwBadValues(text::TextIterator &text_iterator) const -> void;
        static auto throwUnexpectedTermination(text::TextIterator &text_iterator) -> void;
        static auto throwRangeBeginException(text::TextIterator &text_iterator) -> void;
        static auto throwUnexpectedCharacter(text::TextIterator &text_iterator, char32_t chr)
            -> void;

    public:
        size_t from{};
        size_t to{};
    };
}// namespace cerb::lex::dot_item

#endif /* CERBERUS_PROJECT_RECURRENCE_HPP */
