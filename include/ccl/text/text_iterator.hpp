#ifndef CCL_PROJECT_TEXT_ITERATOR_HPP
#define CCL_PROJECT_TEXT_ITERATOR_HPP

#include "basic_text_iterator.hpp"
#include <ccl/handler/exception_handler.hpp>
#include <ccl/pair.hpp>
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
            string_view input, ExceptionHandler &exception_handler_ = ExceptionHandler::instance(),
            string_view filename = {})
          : Base(input), location(filename), line_tracker(input),
            exception_handler(&exception_handler_)
        {}

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

        [[nodiscard]] auto getTabsAndSpaces() const noexcept -> const std::u32string &
        {
            return ts_tracker.get();
        }

        [[nodiscard]] auto getHandler() noexcept -> ExceptionHandler &
        {
            return *exception_handler;
        }

        auto nextRawCharWithEscapingSymbols(const extra_symbols_t &extra_symbols = {})
            -> Pair<bool, char32_t>;

        auto onMove(char chr) -> void;
        auto onCharacter(char32_t chr) -> void;
        auto utfError(char chr) -> void;

        auto throwSuggestion(
            const TextIterator &iterator_location, string_view message, string_view suggestion = {})
            -> void
        {
            throwToHandle(iterator_location, ExceptionCriticality::SUGGESTION, message, suggestion);
        }

        auto throwWarning(
            const TextIterator &iterator_location, string_view message, string_view suggestion = {})
            -> void
        {
            throwToHandle(iterator_location, ExceptionCriticality::WARNING, message, suggestion);
        }

        auto throwUncriticalError(
            const TextIterator &iterator_location, string_view message, string_view suggestion = {})
            -> void
        {
            throwToHandle(iterator_location, ExceptionCriticality::SUGGESTION, message, suggestion);
        }

        auto throwCriticalError(
            const TextIterator &iterator_location, string_view message, string_view suggestion = {})
            -> void
        {
            throwToHandle(iterator_location, ExceptionCriticality::CRITICAL, message, suggestion);
        }

        auto throwPanicError(
            const TextIterator &iterator_location, string_view message, string_view suggestion = {})
            -> void
        {
            throwToHandle(iterator_location, ExceptionCriticality::PANIC, message, suggestion);
        }

        auto throwSuggestion(string_view message, string_view suggestion = {}) -> void
        {
            throwToHandle(*this, ExceptionCriticality::SUGGESTION, message, suggestion);
        }

        auto throwWarning(string_view message, string_view suggestion = {}) -> void
        {
            throwToHandle(*this, ExceptionCriticality::WARNING, message, suggestion);
        }

        auto throwUncriticalError(string_view message, string_view suggestion = {}) -> void
        {
            throwToHandle(*this, ExceptionCriticality::SUGGESTION, message, suggestion);
        }

        auto throwCriticalError(string_view message, string_view suggestion = {}) -> void
        {
            throwToHandle(*this, ExceptionCriticality::CRITICAL, message, suggestion);
        }

        auto throwPanicError(string_view message, string_view suggestion = {}) -> void
        {
            throwToHandle(*this, ExceptionCriticality::PANIC, message, suggestion);
        }

        auto throwToHandle(
            const TextIterator &iterator_location, ExceptionCriticality criticality,
            string_view message, string_view suggestion = {}) -> void;

    private:
        Location location{};
        module::TsTracker ts_tracker{};
        module::LineTracker line_tracker;
        ExceptionHandler *exception_handler{};
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
            extra_symbols_t
                extra_symbols_) noexcept(std::is_nothrow_move_constructible_v<extra_symbols_t>)
          : extra_symbols{ std::move(extra_symbols_) }, text_iterator{ text_iterator_ }
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
            bool need_all_chars_) noexcept;

        ~NotationEscapingSymbolizer() = default;

    private:
        auto calculateResult() -> void;

        [[nodiscard]] auto isOutOfNotation(char32_t chr) const -> bool;

        auto checkNotation() const -> void;
        auto checkAllCharsUsage(u16 chars_count) const -> void;

        auto throwNotEnoughCharsException(u16 chars_count) const -> void;

        [[nodiscard]] auto createSuggestionNotEnoughChars(u16 chars_count) const -> std::string;

        auto insertExtraZerosToNotEnoughMessage(u16 chars_count, std::string &message) const
            -> void;

        TextIterator &text_iterator;
        char32_t result{};
        u16 max_times;
        u16 notation_power;
        bool need_all_chars;
    };
}// namespace ccl::text

#endif /* CCL_PROJECT_TEXT_ITERATOR_HPP */
