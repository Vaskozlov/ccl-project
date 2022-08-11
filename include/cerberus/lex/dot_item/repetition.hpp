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

        explicit Repetition(text::TextIterator &text_iterator)
        {
            checkRangeStart(text_iterator);

            auto iterator_copy = text_iterator;

            from = parseNumber(text_iterator, U',');// NOLINT initialization via =
            to = parseNumber(text_iterator, U'}');  // NOLINT

            checkCorrectnessOfValues(iterator_copy);
        }

    private:
        static auto parseNumber(text::TextIterator &text_iterator, char32_t terminator) -> size_t
        {
            auto result = static_cast<size_t>(0);
            constexpr auto decimal_base = static_cast<size_t>(10);

            text_iterator.skipCommentsAndLayout();

            while (text_iterator.nextCleanChar() != terminator) {
                auto chr = text_iterator.getCurrentChar();

                if (isDigit(chr)) {
                    result = result * decimal_base + static_cast<size_t>(chr - U'0');
                    continue;
                }

                throwUnexpectedCharacter(text_iterator, chr);
            }

            return result;
        }

        static auto checkRangeStart(text::TextIterator &text_iterator) -> void
        {
            if (text_iterator.getCurrentChar() != U'{') {
                throwRangeBeginException(text_iterator);
            }
        }

        auto checkCorrectnessOfValues(text::TextIterator &text_iterator) const -> void
        {
            if (from > to) {
                throwBadValues(text_iterator);
            }
        }

        auto throwBadValues(text::TextIterator &text_iterator) const -> void
        {
            auto message = fmt::format<
                u8"the beginning of the repetition ({}) is greater than the end "
                "({})">(from, to);

            text_iterator.template throwException<RepetitionException>(message);
            throw UnrecoverableError{ "unrecoverable error in Repetition" };
        }

        static auto throwUnexpectedCharacter(text::TextIterator &text_iterator, char32_t chr)
            -> void
        {
            auto message = fmt::format<u8"expected a number, but found `{}`">(chr);

            text_iterator.template throwException<RepetitionException>(message);
            throw UnrecoverableError{ "unrecoverable error in Repetition" };
        }

        static auto throwUnexpectedTermination(text::TextIterator &text_iterator) -> void
        {
            using namespace std::string_view_literals;

            text_iterator.template throwException<RepetitionException>(
                u8"unexpected termination"sv);
            throw UnrecoverableError{ "unrecoverable error in Repetition" };
        }

        static auto throwRangeBeginException(text::TextIterator &text_iterator) -> void
        {
            using namespace std::string_view_literals;

            text_iterator.template throwException<RepetitionException>(
                u8"expected '{' at the beginning of repetition range"sv);
            throw UnrecoverableError{ "unrecoverable error in Repetition" };
        }

    public:
        size_t from{};
        size_t to{};
    };
}// namespace cerb::lex::dot_item

#endif /* CERBERUS_PROJECT_REPETITION_HPP */
