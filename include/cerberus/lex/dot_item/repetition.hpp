#ifndef CERBERUS_PROJECT_REPETITION_HPP
#define CERBERUS_PROJECT_REPETITION_HPP

#include <cerberus/lex/typedefs.hpp>
#include <cerberus/text/text_iterator.hpp>

namespace cerb::lex::dot_item
{
    template<CharacterLiteral CharT>
    CERBLIB_EXCEPTION(RepetitionException, text::TextIteratorException<CharT>);

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

        CERBLIB_DECL auto operator<=>(const Repetition &) const noexcept
            -> std::weak_ordering = default;

        constexpr Repetition(size_t from_, size_t to_) noexcept : from(from_), to(to_)
        {}

        template<CharacterLiteral CharT>
        constexpr explicit Repetition(text::TextIterator<CharT> &text_iterator)
        {
            checkRangeStart(text_iterator);

            auto iterator_copy = text_iterator;

            from = parseNumber(text_iterator, ',');// NOLINT initialization via =
            to = parseNumber(text_iterator, '}');  // NOLINT

            checkCorrectnessOfValues(iterator_copy);
        }

    private:
        template<CharacterLiteral CharT>
        constexpr static auto
            parseNumber(text::TextIterator<CharT> &text_iterator, CharT terminator) -> size_t
        {
            auto result = static_cast<size_t>(0);
            constexpr auto decimal_base = static_cast<size_t>(10);

            text_iterator.skipCommentsAndLayout();

            while (text_iterator.nextCleanChar() != terminator) {
                auto chr = text_iterator.getCurrentChar();

                if (isDigit(chr)) {
                    result = result * decimal_base + static_cast<size_t>(chr - '0');
                } else {
                    throwUnexpectedCharacter(text_iterator, chr);
                }
            }

            return result;
        }

        template<CharacterLiteral CharT>
        constexpr static auto checkRangeStart(text::TextIterator<CharT> &text_iterator) -> void
        {
            if (text_iterator.getCurrentChar() != '{') {
                throwRangeBeginException(text_iterator);
            }
        }

        template<CharacterLiteral CharT>
        constexpr auto checkCorrectnessOfValues(text::TextIterator<CharT> &text_iterator) const
            -> void
        {
            if (from > to) {
                throwBadValues(text_iterator);
            }
        }

        template<CharacterLiteral CharT>
        constexpr auto throwBadValues(text::TextIterator<CharT> &text_iterator) const -> void
        {
            auto message =
                fmt::format<CharT, "Beginning of the range ({}) is greater than end ({})">(
                    from, to);
            text_iterator.throwException(RepetitionException<CharT>(text_iterator, message));
        }

        template<CharacterLiteral CharT>
        constexpr static auto
            throwUnexpectedCharacter(text::TextIterator<CharT> &text_iterator, CharT chr) -> void
        {
            auto message = fmt::format<CharT, "expected a number, but found `{}`">(chr);
            text_iterator.throwException(RepetitionException<CharT>(text_iterator, message));
        }

        template<CharacterLiteral CharT>
        constexpr static auto throwUnexpectedTermination(text::TextIterator<CharT> &text_iterator)
            -> void
        {
            using namespace string_view_literals;

            text_iterator.throwException(
                RepetitionException<CharT>(text_iterator, "unexpected termination"_sv));
        }

        template<CharacterLiteral CharT>
        constexpr static auto throwRangeBeginException(text::TextIterator<CharT> &text_iterator)
            -> void
        {
            using namespace string_view_literals;

            text_iterator.throwException(RepetitionException<CharT>(
                text_iterator, "expected '{' at the beginning of repetition range"_sv));
        }

    public:
        size_t from{};
        size_t to{};
    };
}// namespace cerb::lex::dot_item

#endif /* CERBERUS_PROJECT_REPETITION_HPP */
