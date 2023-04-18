#ifndef CCL_PROJECT_TEXT_ITERATOR_HPP
#define CCL_PROJECT_TEXT_ITERATOR_HPP

#include <ccl/handler/exception_handler.hpp>
#include <ccl/text/basic_text_iterator.hpp>
#include <ccl/text/iterator_exception.hpp>
#include <ccl/text/location.hpp>
#include <ccl/text/text_iterator_modules/line_tracker.hpp>
#include <ccl/text/text_iterator_modules/ts_tracker.hpp>
#include <utility>

namespace ccl::text
{
    class [[nodiscard]] TextIterator : public CrtpBasicTextIterator<TextIterator>
    {
    private:
        using extra_symbols_t = std::basic_string<Pair<char32_t, char32_t>>;

        Location location{};
        TsTracker tsTracker{};
        LineTracker lineTracker;
        ExceptionHandler *exceptionHandler{};

    public:
        class EscapingSymbolizer;
        class NotationEscapingSymbolizer;

        [[nodiscard]] static auto
            doEscapeSymbolizing(TextIterator &text_iterator, const extra_symbols_t &extra_symbols)
                -> char32_t;

        [[nodiscard]] static auto calculateNotationEscapeSymbol(
            TextIterator &text_iterator, u16 max_times, u16 notation_power, bool need_all_chars)
            -> char32_t;

        [[nodiscard]] explicit TextIterator(
            string_view input,
            ExceptionHandler &exception_handler = ExceptionHandler::instance(),
            string_view filename = {});

        [[nodiscard]] explicit TextIterator(
            string_view input,
            ExceptionHandler &exception_handler,
            const Location &iterator_location);

        [[nodiscard]] auto getLocation() const noexcept -> const Location &
        {
            return location;
        }

        [[nodiscard]] auto getLine() const noexcept -> size_t
        {
            return location.getLine();
        }

        [[nodiscard]] auto getColumn() const noexcept -> size_t
        {
            return location.getColumn();
        }

        [[nodiscard]] auto getRealColumn() const noexcept -> size_t
        {
            return location.getRealColumn();
        }

        [[nodiscard]] auto getFilename() const noexcept -> string_view
        {
            return location.getFilename();
        }

        [[nodiscard]] auto getWorkingLine() const noexcept -> string_view
        {
            return lineTracker.get();
        }

        [[nodiscard]] auto getTabsAndSpaces() const noexcept -> const std::string &
        {
            return tsTracker.get();
        }

        [[nodiscard]] auto getHandler() noexcept -> ExceptionHandler &
        {
            return *exceptionHandler;
        }

        auto nextRawCharWithEscapingSymbols(const extra_symbols_t &extra_symbols = {})
            -> Pair<bool, char32_t>;

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
            AnalysisStage stage, const TextIterator &iterator_location, string_view message,
            string_view suggestion = {}) -> void;

        auto throwWarning(
            AnalysisStage stage, const TextIterator &iterator_location, string_view message,
            string_view suggestion = {}) -> void;

        auto throwUncriticalError(
            AnalysisStage stage, const TextIterator &iterator_location, string_view message,
            string_view suggestion = {}) -> void;

        auto throwCriticalError(
            AnalysisStage stage, const TextIterator &iterator_location, string_view message,
            string_view suggestion = {}) -> void;

        auto throwPanicError(
            AnalysisStage stage, const TextIterator &iterator_location, string_view message,
            string_view suggestion = {}) -> void;

        auto throwSuggestion(AnalysisStage stage, string_view message, string_view suggestion = {})
            -> void;

        auto throwWarning(AnalysisStage stage, string_view message, string_view suggestion = {})
            -> void;

        auto throwUncriticalError(
            AnalysisStage stage, string_view message, string_view suggestion = {}) -> void;

        auto throwCriticalError(
            AnalysisStage stage, string_view message, string_view suggestion = {}) -> void;

        auto throwPanicError(AnalysisStage stage, string_view message, string_view suggestion = {})
            -> void;

        auto throwToHandle(
            const TextIterator &iterator_location, ExceptionCriticality criticality,
            AnalysisStage stage, string_view message, string_view suggestion = {}) -> void;
    };

    class TextIterator::EscapingSymbolizer
    {
    public:
        [[nodiscard]] auto getExtraSymbols() const noexcept -> const extra_symbols_t &
        {
            return extraSymbols;
        }

        [[nodiscard]] auto matchNextChar() -> char32_t;

        auto operator=(EscapingSymbolizer &&) -> void = delete;
        auto operator=(const EscapingSymbolizer &) -> void = delete;

        EscapingSymbolizer() = delete;
        EscapingSymbolizer(EscapingSymbolizer &&) = delete;
        EscapingSymbolizer(const EscapingSymbolizer &) = delete;

        explicit EscapingSymbolizer(
            TextIterator &text_iterator,
            extra_symbols_t extra_symbols) noexcept
          : extraSymbols{std::move(extra_symbols)}
          , textIterator{text_iterator}
        {}

        ~EscapingSymbolizer() = default;

    private:
        auto searchInExtraSymbols(char32_t chr) -> char32_t;
        auto throwMatchException() -> void;

        extra_symbols_t extraSymbols;
        TextIterator &textIterator;
    };

    class TextIterator::NotationEscapingSymbolizer
    {
    public:
        CCL_DECL auto get() const noexcept -> char32_t
        {
            return result;
        }

        auto operator=(NotationEscapingSymbolizer &&) -> void = delete;
        auto operator=(const NotationEscapingSymbolizer &) -> void = delete;

        NotationEscapingSymbolizer() = delete;
        NotationEscapingSymbolizer(NotationEscapingSymbolizer &&) = delete;
        NotationEscapingSymbolizer(const NotationEscapingSymbolizer &) = delete;

        NotationEscapingSymbolizer(
            TextIterator &text_iterator, u16 maximum_symbols, u16 notation_power,
            bool are_all_chars_required);

        ~NotationEscapingSymbolizer() = default;

    private:
        auto calculateResult() -> void;

        [[nodiscard]] auto isOutOfNotation(char32_t chr) const -> bool;

        auto checkNotation() const -> void;
        auto checkAllCharsUsage(size_t chars_count) const -> void;

        auto throwNotEnoughCharsException(size_t chars_count) const -> void;

        [[nodiscard]] auto createSuggestionNotEnoughChars(size_t chars_count) const -> std::string;

        auto insertExtraZerosToNotEnoughMessage(size_t chars_count, std::string &message) const
            -> void;

        TextIterator &textIterator;
        char32_t result{};
        u16 maximumSymbols;
        u16 notationPower;
        bool areAllCharsRequired;
    };
}// namespace ccl::text

#endif /* CCL_PROJECT_TEXT_ITERATOR_HPP */
