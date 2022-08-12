#ifndef CERBERUS_PROJECT_REPETITION_HPP
#define CERBERUS_PROJECT_REPETITION_HPP

#include <cerberus/text/text_iterator.hpp>

namespace cerb::lex::dot_item
{
    CERBLIB_EXCEPTION(RepetitionException, text::TextIteratorException);

    struct CERBLIB_TRIVIAL_ABI Repetition
    {
        consteval static auto basic() -> Repetition
        {
            return { 1, 1 };
        }

        consteval static auto question() -> Repetition
        {
            return { 0, 1 };
        }

        consteval static auto star() -> Repetition
        {
            return { 0, std::numeric_limits<size_t>::max() };
        }

        consteval static auto plus() -> Repetition
        {
            return { 1, std::numeric_limits<size_t>::max() };
        }

        [[nodiscard]] auto inRange(size_t value) const noexcept
        {
            return land(value >= from, value <= to);
        }

        [[nodiscard]] auto operator<=>(const Repetition &) const noexcept
            -> std::weak_ordering = default;

        constexpr Repetition(size_t from_, size_t to_) noexcept : from(from_), to(to_)
        {}

        explicit Repetition(text::TextIterator &text_iterator);

    private:
        static auto parseNumber(text::TextIterator &text_iterator, char32_t terminator) -> size_t;

        static auto checkRangeStart(text::TextIterator &text_iterator) -> void;
        auto checkCorrectnessOfValues(text::TextIterator &text_iterator) const -> void;

        auto throwBadValues(text::TextIterator &text_iterator) const -> void;
        static auto throwUnexpectedCharacter(text::TextIterator &text_iterator, char32_t chr)
            -> void;
        static auto throwUnexpectedTermination(text::TextIterator &text_iterator) -> void;
        static auto throwRangeBeginException(text::TextIterator &text_iterator) -> void;

    public:
        size_t from{};
        size_t to{};
    };
}// namespace cerb::lex::dot_item

#endif /* CERBERUS_PROJECT_REPETITION_HPP */
