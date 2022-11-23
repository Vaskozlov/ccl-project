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
    class TextIterator : public CrtpBasicTextIterator<TextIterator>
    {
    private:
        using Base = CrtpBasicTextIterator<TextIterator>;
        using extra_symbols_t = std::basic_string<Pair<char32_t, char32_t>>;

        Location location{};
        TsTracker ts_tracker{};
        LineTracker line_tracker;
        ExceptionHandler *exception_handler{};

    public:
        class EscapingSymbolizer;
        class NotationEscapingSymbolizer;

        [[nodiscard]] static auto
            doEscapeSymbolizing(TextIterator &text_iterator, const extra_symbols_t &extra_symbols_)
                -> char32_t;

        [[nodiscard]] static auto calculateNotationEscapeSymbol(
            TextIterator &text_iterator, u16 max_times, u16 notation_power, bool need_all_chars)
            -> char32_t;

        explicit TextIterator(
            const string_view &input,
            ExceptionHandler &exception_handler_ = ExceptionHandler::instance(),
            const string_view &filename = {});

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

        [[nodiscard]] auto getFilename() const noexcept -> const string_view &
        {
            return location.getFilename();
        }

        [[nodiscard]] auto getWorkingLine() const noexcept -> const string_view &
        {
            return line_tracker.get();
        }

        [[nodiscard]] auto getTabsAndSpaces() const noexcept -> const std::string &
        {
            return ts_tracker.get();
        }

        [[nodiscard]] auto getHandler() noexcept -> ExceptionHandler &
        {
            return *exception_handler;
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
            ts_tracker.next(chr);
            line_tracker.next(chr);
        }

        auto utfError(char /* unused */) -> void
        {
            throwPanicError(AnalysationStage::LEXICAL_ANALYSIS, "invalid utf symbol");
            throw UnrecoverableError{"unable to recover, because of invalid utf symbol"};
        }

        auto throwSuggestion(
            AnalysationStage stage, const TextIterator &iterator_location,
            const string_view &message, const string_view &suggestion = {}) -> void
        {
            throwToHandle(
                iterator_location, ExceptionCriticality::SUGGESTION, stage, message, suggestion);
        }

        auto throwWarning(
            AnalysationStage stage, const TextIterator &iterator_location,
            const string_view &message, const string_view &suggestion = {}) -> void
        {
            throwToHandle(
                iterator_location, ExceptionCriticality::WARNING, stage, message, suggestion);
        }

        auto throwUncriticalError(
            AnalysationStage stage, const TextIterator &iterator_location,
            const string_view &message, const string_view &suggestion = {}) -> void
        {
            throwToHandle(
                iterator_location, ExceptionCriticality::SUGGESTION, stage, message, suggestion);
        }

        auto throwCriticalError(
            AnalysationStage stage, const TextIterator &iterator_location,
            const string_view &message, const string_view &suggestion = {}) -> void
        {
            throwToHandle(
                iterator_location, ExceptionCriticality::CRITICAL, stage, message, suggestion);
        }

        auto throwPanicError(
            AnalysationStage stage, const TextIterator &iterator_location,
            const string_view &message, const string_view &suggestion = {}) -> void
        {
            throwToHandle(
                iterator_location, ExceptionCriticality::PANIC, stage, message, suggestion);
        }

        auto throwSuggestion(
            AnalysationStage stage, const string_view &message, const string_view &suggestion = {})
            -> void
        {
            throwToHandle(*this, ExceptionCriticality::SUGGESTION, stage, message, suggestion);
        }

        auto throwWarning(
            AnalysationStage stage, const string_view &message, const string_view &suggestion = {})
            -> void
        {
            throwToHandle(*this, ExceptionCriticality::WARNING, stage, message, suggestion);
        }

        auto throwUncriticalError(
            AnalysationStage stage, const string_view &message, const string_view &suggestion = {})
            -> void
        {
            throwToHandle(*this, ExceptionCriticality::SUGGESTION, stage, message, suggestion);
        }

        auto throwCriticalError(
            AnalysationStage stage, const string_view &message, const string_view &suggestion = {})
            -> void
        {
            throwToHandle(*this, ExceptionCriticality::CRITICAL, stage, message, suggestion);
        }

        auto throwPanicError(
            AnalysationStage stage, const string_view &message, const string_view &suggestion = {})
            -> void
        {
            throwToHandle(*this, ExceptionCriticality::PANIC, stage, message, suggestion);
        }

        auto throwToHandle(
            const TextIterator &iterator_location, ExceptionCriticality criticality,
            AnalysationStage stage, const string_view &message, const string_view &suggestion = {})
            -> void;
    };

    class TextIterator::EscapingSymbolizer
    {
    public:
        [[nodiscard]] auto getExtraSymbols() const noexcept -> const extra_symbols_t &
        {
            return extra_symbols;
        }

        [[nodiscard]] auto matchNextChar() -> char32_t;

        auto operator=(EscapingSymbolizer &&) -> void = delete;
        auto operator=(const EscapingSymbolizer &) -> void = delete;

        EscapingSymbolizer() = delete;
        EscapingSymbolizer(EscapingSymbolizer &&) = delete;
        EscapingSymbolizer(const EscapingSymbolizer &) = delete;

        explicit EscapingSymbolizer(
            TextIterator &text_iterator_,
            extra_symbols_t extra_symbols_) noexcept
          : extra_symbols{std::move(extra_symbols_)}
          , text_iterator{text_iterator_}
        {}

        ~EscapingSymbolizer() = default;

    private:
        auto searchInExtraSymbols(char32_t chr) -> char32_t;
        auto throwMatchException() -> void;

        extra_symbols_t extra_symbols;
        TextIterator &text_iterator;
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
            TextIterator &text_iterator_, u16 max_times_, u16 notation_power_,
            bool are_all_chars_required_);

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
