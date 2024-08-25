#ifndef CCL_PROJECT_TEXT_ITERATOR_HPP
#define CCL_PROJECT_TEXT_ITERATOR_HPP

#include <ccl/handler/exception_handler.hpp>
#include <ccl/text/basic_text_iterator.hpp>
#include <ccl/text/iterator_exception.hpp>
#include <ccl/text/location.hpp>
#include <span>
#include <utility>

namespace ccl::text
{
    class TextIterator : public CrtpBasicTextIterator<TextIterator>
    {
    private:
        using extra_symbols_t = std::span<const isl::Pair<char32_t, char32_t>>;

        Location location;
        isl::string_view wholeInput;
        ExceptionHandler *exceptionHandler{};

    public:
        class EscapingSequenceToChar;
        class NotationEscapingSequenceToChar;

        [[nodiscard]] static auto doEscapeSymbolizing(
            TextIterator &text_iterator, const extra_symbols_t &extra_symbols) -> char32_t;

        [[nodiscard]] static auto calculateNotationEscapeSymbol(
            TextIterator &text_iterator, u16 max_times, u16 notation_power,
            bool need_all_chars) -> char32_t;

        [[nodiscard]] explicit TextIterator(
            isl::string_view input,
            ExceptionHandler &exception_handler = ExceptionHandler::instance(),
            isl::string_view filename = {});

        [[nodiscard]] explicit TextIterator(
            isl::string_view input,
            ExceptionHandler &exception_handler,
            const Location &iterator_location);

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

        [[nodiscard]] auto getWholeInput() const noexcept -> isl::string_view
        {
            return wholeInput;
        }

        [[nodiscard]] auto getCurrentLine() const -> isl::string_view
        {
            const auto *current_it = getCarriage();
            return linesOfFragment(wholeInput, {current_it, std::next(current_it)});
        }

        [[nodiscard]] auto getHandler() noexcept -> ExceptionHandler &
        {
            return *exceptionHandler;
        }

        auto nextCharWithEscapingSymbols(const extra_symbols_t &extra_symbols = {})
            -> isl::Pair<bool, char32_t>;

        CCL_INLINE auto onMove(char chr) -> void
        {
            location.intermediateNext(chr);
        }

        CCL_INLINE auto onCharacter(char32_t chr) -> void
        {
            location.next(chr);
        }

        [[noreturn]] auto utfError(char /* unused */) -> void
        {
            throwPanicError(AnalysisStage::LEXICAL_ANALYSIS, "invalid utf symbol");
            throw UnrecoverableError{"unable to recover, because of invalid utf symbol"};
        }

        auto throwSuggestion(
            AnalysisStage stage, const TextIterator &iterator_location, isl::string_view message,
            isl::string_view suggestion = {}) -> void;

        auto throwWarning(
            AnalysisStage stage, const TextIterator &iterator_location, isl::string_view message,
            isl::string_view suggestion = {}) -> void;

        auto throwUncriticalError(
            AnalysisStage stage, const TextIterator &iterator_location, isl::string_view message,
            isl::string_view suggestion = {}) -> void;

        auto throwCriticalError(
            AnalysisStage stage, const TextIterator &iterator_location, isl::string_view message,
            isl::string_view suggestion = {}) -> void;

        auto throwPanicError(
            AnalysisStage stage, const TextIterator &iterator_location, isl::string_view message,
            isl::string_view suggestion = {}) -> void;

        auto throwSuggestion(
            AnalysisStage stage, isl::string_view message,
            isl::string_view suggestion = {}) -> void;

        auto throwWarning(
            AnalysisStage stage, isl::string_view message,
            isl::string_view suggestion = {}) -> void;

        auto throwUncriticalError(
            AnalysisStage stage, isl::string_view message,
            isl::string_view suggestion = {}) -> void;

        auto throwCriticalError(
            AnalysisStage stage, isl::string_view message,
            isl::string_view suggestion = {}) -> void;

        auto throwPanicError(
            AnalysisStage stage, isl::string_view message,
            isl::string_view suggestion = {}) -> void;

        auto throwToHandle(
            const TextIterator &iterator_location, ExceptionCriticality criticality,
            AnalysisStage stage, isl::string_view message,
            isl::string_view suggestion = {}) -> void;

        static auto linesOfFragment(isl::string_view whole_input, isl::string_view fragment)
            -> isl::string_view;
    };

    class TextIterator::EscapingSequenceToChar
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
            extra_symbols_t extra_symbols) noexcept;

        ~EscapingSequenceToChar() = default;

        auto operator=(EscapingSequenceToChar &&) -> void = delete;
        auto operator=(const EscapingSequenceToChar &) -> void = delete;

        [[nodiscard]] auto
            getExtraSymbols() const noexcept CCL_LIFETIMEBOUND -> const extra_symbols_t &
        {
            return extraSymbols;
        }

        [[nodiscard]] auto matchNextChar() -> char32_t;

    private:
        auto searchInExtraSymbols(char32_t chr) -> char32_t;
        auto throwUnableToMatchEscapingSymbol() -> void;
    };

    class TextIterator::NotationEscapingSequenceToChar
    {
    private:
        TextIterator &textIterator;
        char32_t result{};
        u16 maximumSymbols{};
        u16 notationPower{};
        bool areAllCharsRequired{};

    public:
        NotationEscapingSequenceToChar(
            TextIterator &text_iterator, u16 maximum_symbols, u16 notation_power,
            bool are_all_chars_required);

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
        auto calculateResult() -> void;

        [[nodiscard]] auto isOutOfNotation(char32_t chr) const -> bool;

        auto checkNotation() const -> void;
        auto checkAllCharsUsage(std::size_t chars_count) const -> void;

        auto throwNotEnoughCharsException(std::size_t chars_count) const -> void;

        [[nodiscard]] auto
            createSuggestionNotEnoughChars(std::size_t chars_count) const -> std::string;

        auto insertExtraZerosToNotEnoughMessage(std::size_t chars_count, std::string &message) const
            -> void;
    };

    extern template class CrtpBasicTextIterator<TextIterator>;
}// namespace ccl::text

#endif /* CCL_PROJECT_TEXT_ITERATOR_HPP */
