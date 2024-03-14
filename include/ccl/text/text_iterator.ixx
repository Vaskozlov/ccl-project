module;

#include <ccl/defines.hpp>

export module ccl.text:text_iterator;

export import ccl.core;
export import ccl.text.modules;
export import ccl.handler;
export import ccl.text.core;
export import :basic_text_iterator;

export namespace ccl::text
{
    class TextIterator : public CrtpBasicTextIterator<TextIterator>
    {
    private:
        using extra_symbols_t = std::span<const isl::Pair<char32_t, char32_t>>;

        Location location;
        modules::TsTracker tsTracker;
        modules::LineTracker lineTracker;
        handler::ExceptionHandler *exceptionHandler{};

    public:
        class EscapingSequenceToChar
        {
        private:
            extra_symbols_t extraSymbols;
            TextIterator &textIterator;

        public:
            EscapingSequenceToChar() = delete;
            EscapingSequenceToChar(EscapingSequenceToChar &&) = delete;
            EscapingSequenceToChar(const EscapingSequenceToChar &) = delete;

            [[nodiscard]] explicit EscapingSequenceToChar(
                TextIterator &text_iterator,
                extra_symbols_t extra_symbols) noexcept
              : extraSymbols{extra_symbols}
              , textIterator{text_iterator}
            {}

            ~EscapingSequenceToChar() = default;

            auto operator=(EscapingSequenceToChar &&) -> void = delete;
            auto operator=(const EscapingSequenceToChar &) -> void = delete;

            [[nodiscard]] auto
                getExtraSymbols() const noexcept CCL_LIFETIMEBOUND -> const extra_symbols_t &
            {
                return extraSymbols;
            }

            [[nodiscard]] auto matchNextChar() -> char32_t
            {
                ISL_ASSERT_MSG(
                    textIterator.getCurrentChar() == '\\', "called without preceding `\\`");

                char32_t chr = textIterator.advance();

                switch (chr) {
                case U'\\':
                    return U'\\';

                case U'\'':
                    return U'\'';

                case U'\"':
                    return U'\"';

                case U'n':
                    return U'\n';

                case U't':
                    return U'\t';

                case U'r':
                    return U'\r';

                case U'f':
                    return U'\f';

                case U'b':
                    return U'\b';

                case U'v':
                    return U'\v';

                case U'a':
                    return U'\a';

                case U'0':
                    return calculateNotationEscapeSymbol(textIterator, 2, 3, false);

                case U'x':
                    return calculateNotationEscapeSymbol(textIterator, 2, 4, true);

                case U'u':
                    return calculateNotationEscapeSymbol(textIterator, 4, 4, true);

                case U'U':
                    return calculateNotationEscapeSymbol(textIterator, 8, 4, true);// NOLINT

                default:
                    return searchInExtraSymbols(chr);
                }
            }

        private:
            auto searchInExtraSymbols(char32_t chr) -> char32_t
            {
                auto it = std::find_if(extraSymbols.begin(), extraSymbols.end(), [chr](auto elem) {
                    return elem.first == chr;
                });

                if (it == extraSymbols.end()) [[unlikely]] {
                    throwUnableToMatchEscapingSymbol();
                    return U'?';
                }

                return it->second;
            }

            auto throwUnableToMatchEscapingSymbol() -> void
            {
                textIterator.throwUncriticalError(
                    AnalysisStage::LEXICAL_ANALYSIS, "unable to matchNextChar any escaping symbol");
            }
        };

        class NotationEscapingSequenceToChar
        {
        private:
            TextIterator &textIterator;
            char32_t result{};
            u16 maximumSymbols;
            u16 notationPower;
            bool areAllCharsRequired;

        public:
            NotationEscapingSequenceToChar(
                TextIterator &text_iterator, u16 maximum_symbols, u16 notation_power,
                bool are_all_chars_required)
              : textIterator{text_iterator}
              , maximumSymbols{maximum_symbols}
              , notationPower{notation_power}
              , areAllCharsRequired{are_all_chars_required}
            {
                checkNotation();
                calculateResult();
            }

            NotationEscapingSequenceToChar() = delete;
            NotationEscapingSequenceToChar(NotationEscapingSequenceToChar &&) = delete;
            NotationEscapingSequenceToChar(const NotationEscapingSequenceToChar &) = delete;

            ~NotationEscapingSequenceToChar() = default;

            auto operator=(NotationEscapingSequenceToChar &&) -> void = delete;
            auto operator=(const NotationEscapingSequenceToChar &) -> void = delete;

            CCL_DECL auto get() const noexcept -> char32_t
            {
                return result;
            }

        private:
            auto calculateResult() -> void
            {
                auto chars_count = 0ZU;

                for (; chars_count != maximumSymbols; ++chars_count) {
                    char32_t chr = textIterator.futureChar();

                    if (isEoF(chr) || isOutOfNotation(chr)) [[unlikely]] {
                        break;
                    }

                    result = isl::as<char32_t>(result << notationPower);
                    result += isl::as<char32_t>(HexadecimalCharsToInt.at(chr));

                    textIterator.advance();
                }

                checkAllCharsUsage(chars_count);
            }

            [[nodiscard]] auto isOutOfNotation(char32_t chr) const -> bool
            {
                return !HexadecimalCharsToInt.contains(chr) ||
                       HexadecimalCharsToInt.at(chr) >= (1U << notationPower);
            }

            auto checkNotation() const -> void
            {
                ISL_ASSERT(notationPower > 0 && notationPower <= 4);
            }

            auto checkAllCharsUsage(std::size_t chars_count) const -> void
            {
                if (areAllCharsRequired && chars_count != maximumSymbols) {
                    throwNotEnoughCharsException(chars_count);
                }
            }

            auto throwNotEnoughCharsException(std::size_t chars_count) const -> void
            {
                auto exception_message = std::format(
                    "expected {} characters, but only {} of them were provided", maximumSymbols,
                    chars_count);

                auto suggestion_message = createSuggestionNotEnoughChars(chars_count);

                textIterator.throwUncriticalError(
                    AnalysisStage::LEXICAL_ANALYSIS, exception_message, suggestion_message);
            }

            [[nodiscard]] auto
                createSuggestionNotEnoughChars(std::size_t chars_count) const -> std::string
            {
                auto suggestion_message = as<std::string>(textIterator.getWorkingLine());
                insertExtraZerosToNotEnoughMessage(chars_count, suggestion_message);
                return suggestion_message;
            }

            auto insertExtraZerosToNotEnoughMessage(
                std::size_t chars_count, std::string &message) const -> void
            {
                auto column = textIterator.getColumn();
                auto insertion_size = isl::as<std::size_t>(maximumSymbols - chars_count);
                auto insertion_position = column - chars_count;

                message.insert(insertion_position, insertion_size, '0');
            }
        };

        [[nodiscard]] static auto doEscapeSymbolizing(
            TextIterator &text_iterator, const extra_symbols_t &extra_symbols) -> char32_t
        {
            return EscapingSequenceToChar(text_iterator, extra_symbols).matchNextChar();
        }

        [[nodiscard]] static auto calculateNotationEscapeSymbol(
            TextIterator &text_iterator, u16 max_times, u16 notation_power,
            bool need_all_chars) -> char32_t
        {
            return NotationEscapingSequenceToChar(
                       text_iterator, max_times, notation_power, need_all_chars)
                .get();
        }

        [[nodiscard]] explicit TextIterator(
            isl::string_view input,
            handler::ExceptionHandler &exception_handler = handler::ExceptionHandler::instance(),
            isl::string_view filename = {})
          : CrtpBasicTextIterator{input}
          , location{filename}
          , lineTracker{input}
          , exceptionHandler{&exception_handler}
        {}

        [[nodiscard]] explicit TextIterator(
            isl::string_view input,
            handler::ExceptionHandler &exception_handler,
            const Location &iterator_location)
          : CrtpBasicTextIterator{input}
          , location{iterator_location}
          , lineTracker{input}
          , exceptionHandler{&exception_handler}
        {}

        [[nodiscard]] auto getLocation() const noexcept CCL_LIFETIMEBOUND -> const Location &
        {
            return location;
        }

        [[nodiscard]] auto getLine() const noexcept -> std::size_t
        {
            return location.getLine();
        }

        [[nodiscard]] auto getColumn() const noexcept -> std::size_t
        {
            return location.getColumn();
        }

        [[nodiscard]] auto getRealColumn() const noexcept -> std::size_t
        {
            return location.getRealColumn();
        }

        [[nodiscard]] auto getFilename() const noexcept -> isl::string_view
        {
            return location.getFilename();
        }

        [[nodiscard]] auto getWorkingLine() const noexcept -> isl::string_view
        {
            return lineTracker.get();
        }

        [[nodiscard]] auto
            getTabsAndSpaces() const noexcept CCL_LIFETIMEBOUND -> const std::string &
        {
            return tsTracker.get();
        }

        [[nodiscard]] auto getHandler() noexcept -> handler::ExceptionHandler &
        {
            return *exceptionHandler;
        }

        auto nextCharWithEscapingSymbols(const extra_symbols_t &extra_symbols = {})
            -> isl::Pair<bool, char32_t>
        {
            auto escaping = false;
            char32_t chr = advance();

            if (U'\\' == chr) {
                escaping = true;
                chr = doEscapeSymbolizing(*this, extra_symbols);
            }

            CrtpBasicTextIterator::setCurrentChar(chr);
            return {escaping, chr};
        }

        CCL_INLINE auto onMove(char chr) -> void
        {
            location.intermediateNext(chr);
        }

        CCL_INLINE auto onCharacter(char32_t chr) -> void
        {
            location.next(chr);
            tsTracker.next(chr);
            lineTracker.next(chr);
        }

        [[noreturn]] auto utfError(char /* unused */) -> void
        {
            throwPanicError(AnalysisStage::LEXICAL_ANALYSIS, "invalid utf symbol");
            throw UnrecoverableError{"unable to recover, because of invalid utf symbol"};
        }

        auto throwSuggestion(
            AnalysisStage stage, const TextIterator &iterator_location, isl::string_view message,
            isl::string_view suggestion = {}) -> void
        {
            throwToHandle(
                iterator_location, ExceptionCriticality::SUGGESTION, stage, message, suggestion);
        }

        auto throwWarning(
            AnalysisStage stage, const TextIterator &iterator_location, isl::string_view message,
            isl::string_view suggestion = {}) -> void
        {
            throwToHandle(iterator_location, ExceptionCriticality::WARNING, stage, message, suggestion);
        }

        auto throwUncriticalError(
            AnalysisStage stage, const TextIterator &iterator_location, isl::string_view message,
            isl::string_view suggestion = {}) -> void
        {
            throwToHandle(
                iterator_location, ExceptionCriticality::SUGGESTION, stage, message, suggestion);
        }

        auto throwCriticalError(
            AnalysisStage stage, const TextIterator &iterator_location, isl::string_view message,
            isl::string_view suggestion = {}) -> void
        {
            throwToHandle(
                iterator_location, ExceptionCriticality::CRITICAL, stage, message, suggestion);
        }

        auto throwPanicError(
            AnalysisStage stage, const TextIterator &iterator_location, isl::string_view message,
            isl::string_view suggestion = {}) -> void
        {
            throwToHandle(iterator_location, ExceptionCriticality::PANIC, stage, message, suggestion);
        }

        auto throwSuggestion(
            AnalysisStage stage, isl::string_view message,
            isl::string_view suggestion = {}) -> void
        {
            throwToHandle(*this, ExceptionCriticality::SUGGESTION, stage, message, suggestion);
        }

        auto throwWarning(
            AnalysisStage stage, isl::string_view message,
            isl::string_view suggestion = {}) -> void
        {
            throwToHandle(*this, ExceptionCriticality::WARNING, stage, message, suggestion);
        }

        auto throwUncriticalError(
            AnalysisStage stage, isl::string_view message,
            isl::string_view suggestion = {}) -> void
        {
            throwToHandle(*this, ExceptionCriticality::UNCRITICAL, stage, message, suggestion);
        }

        auto throwCriticalError(
            AnalysisStage stage, isl::string_view message,
            isl::string_view suggestion = {}) -> void
        {
            throwToHandle(*this, ExceptionCriticality::CRITICAL, stage, message, suggestion);
        }

        auto throwPanicError(
            AnalysisStage stage, isl::string_view message,
            isl::string_view suggestion = {}) -> void
        {
            throwToHandle(*this, ExceptionCriticality::PANIC, stage, message, suggestion);
        }

        auto throwToHandle(
            const TextIterator &iterator_location, ExceptionCriticality criticality,
            AnalysisStage stage, isl::string_view message,
            isl::string_view suggestion = {}) -> void
        {
            auto exception = TextIteratorException(
                criticality, stage, iterator_location.getLocation(), 1,
                iterator_location.getWorkingLine(), as<std::string>(message),
                as<std::string>(suggestion));

            if (nullptr == exceptionHandler) {
                throw std::move(exception);
            }

            exceptionHandler->handle(exception);
        }
    };
}// namespace ccl::text
