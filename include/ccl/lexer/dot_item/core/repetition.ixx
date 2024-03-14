export module ccl.lexer.dot_item.core:repetition;

export import ccl.text;

export namespace ccl::lexer::dot_item
{
    class Repetition
    {
    public:
        std::size_t from{};
        std::size_t to{};

        [[nodiscard]] constexpr Repetition(
            std::size_t repetition_begin, std::size_t repetition_end) noexcept
          : from{repetition_begin}
          , to{repetition_end}
        {}

        [[nodiscard]] explicit Repetition(text::TextIterator &text_iterator)
        {
            checkRangeStart(text_iterator);
            auto iterator_copy = text_iterator;

            from = parseNumber(text_iterator, ',');// NOLINT initialization via =
            to = parseNumber(text_iterator, '}');  // NOLINT

            checkCorrectnessOfValues(iterator_copy);
        }

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

        [[nodiscard]] consteval static auto max() noexcept -> std::size_t
        {
            return std::numeric_limits<std::size_t>::max();
        }

        [[nodiscard]] auto inRange(std::size_t value) const noexcept
        {
            return value >= from && value <= to;
        }

        [[nodiscard]] auto
            operator<=>(const Repetition &) const noexcept -> std::strong_ordering = default;

    private:
        [[nodiscard]] static auto
            parseNumber(text::TextIterator &text_iterator, char32_t terminator) -> std::size_t
        {
            constexpr auto decimal_base = 10ZU;

            auto result = 0ZU;
            text_iterator.moveToCleanChar();

            while (text_iterator.advance() != terminator) {
                const char32_t chr = text_iterator.getCurrentChar();

                if (text::isDigit(chr)) {
                    result = result * decimal_base + isl::as<std::size_t>(chr - '0');
                    continue;
                }

                throwUnexpectedCharacter(text_iterator, chr);
            }

            return result;
        }

        static auto checkRangeStart(text::TextIterator &text_iterator) -> void
        {
            if (text_iterator.getCurrentChar() != '{') {
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
            auto message = std::format(
                "the beginning of the repetition ({}) is greater than the end "
                "({})",
                from, to);

            text_iterator.throwCriticalError(text::AnalysisStage::LEXICAL_ANALYSIS, message);
        }

        [[noreturn]] static auto throwRangeBeginException(text::TextIterator &text_iterator) -> void
        {
            text_iterator.throwPanicError(
                text::AnalysisStage::LEXICAL_ANALYSIS,
                "expected '{' at the beginning of repetition range");
            throw UnrecoverableError{"unrecoverable error in Repetition"};
        }

        [[noreturn]] static auto
            throwUnexpectedCharacter(text::TextIterator &text_iterator, char32_t chr) -> void
        {
            auto buffer = std::string{};
            isl::utf8::appendUtf32ToUtf8Container(buffer, chr);

            auto message = std::format("expected a number, but found `{}`", buffer);

            text_iterator.throwPanicError(text::AnalysisStage::LEXICAL_ANALYSIS, message);
            throw UnrecoverableError{"unrecoverable error in Repetition"};
        }
    };
}// namespace ccl::lexer::dot_item
