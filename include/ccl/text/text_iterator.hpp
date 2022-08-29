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
    struct CommentTokens
    {
        std::string single_line{};
        std::string multiline_begin{};
        std::string multiline_end{};
    };

    CCL_EXCEPTION(CommentSkipperException, TextIteratorException);
    CCL_EXCEPTION(NotationEscapingSymbolizerException, TextIteratorException);

    class TextIterator : public CrtpBasicTextIterator<TextIterator>
    {
    private:
        using Base = CrtpBasicTextIterator<TextIterator>;
        using extra_symbols_t = std::basic_string<Pair<char32_t, char32_t>>;

    public:
        class CommentSkipper;
        class EscapingSymbolizer;
        class NotationEscapingSymbolizer;

        explicit TextIterator(
            string_view input, ExceptionHandler &exception_handler_ = ExceptionHandler::instance(),
            CommentTokens comment_tokens_ = {}, string_view filename = {})
          : Base(input), location(filename), line_tracker(input),
            comment_tokens(std::move(comment_tokens_)), exception_handler(&exception_handler_)
        {}

        [[nodiscard]] static auto
            doEscapeSymbolizing(TextIterator &text_iterator, const extra_symbols_t &extra_symbols_)
                -> char32_t;

        [[nodiscard]] static auto calculateNotationEscapeSymbol(
            TextIterator &text_iterator, u16 max_times, u16 notation_power, bool need_all_chars)
            -> char32_t;

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

        auto nextRawCharWithEscapingSymbols(const extra_symbols_t &extra_symbols = {})
            -> Pair<bool, char32_t>;

        auto onMove(char chr) -> void;
        auto onCharacter(char32_t chr) -> void;
        auto utfError(char chr) -> void;

        auto skipComments() -> bool;
        auto skipCommentsAndLayout() -> void;

        template<Exception T>
        auto throwError(T &&exception) -> void
        {
            if (exception_handler == nullptr) {
                std::terminate();
            }

            exception_handler->handleError(std::forward<T>(exception));
        }

        template<Exception T = TextIteratorException, typename... Ts>
        auto throwError(Ts &&...args) -> void
        {
            throwError(T{ getLocation(), getWorkingLine(), std::forward<Ts>(args)... });
        }

        template<Exception T>
        auto throwWarning(T &&exception) -> void
        {
            if (exception_handler != nullptr) {
                exception_handler->handleWarning(std::forward<T>(exception));
            }
        }

        template<Exception T, typename... Ts>
        auto throwWarning(Ts &&...args) -> void
        {
            throwWarning(T{ getLocation(), getWorkingLine(), std::forward<Ts>(args)... });
        }

    private:
        Location location{};
        module::TsTracker ts_tracker{};
        module::LineTracker line_tracker;
        CommentTokens comment_tokens{};
        ExceptionHandler *exception_handler{};
    };

    class TextIterator::CommentSkipper
    {
    public:
        auto skip() -> bool;

        CommentSkipper(TextIterator &text_iterator_, const CommentTokens &comment_tokens_);

    private:
        [[nodiscard]] auto isComment(const string_view &comment) const noexcept -> bool;

        auto skipSingleLine() -> void;
        auto skipMultiline() -> void;

        auto checkCommentTermination(const TextIterator &comment_begin) const -> void;

        auto throwUnterminatedCommentError(const TextIterator &comment_begin) const -> void;

        const CommentTokens &comment_tokens;// MAYBE: copy comment tokens into StrViews
        TextIterator &text_iterator;
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
        auto checkCharacterOverflow() const -> void;
        auto checkAllCharsUsage(u16 chars_count) const -> void;

        auto throwCharacterOverflow() const -> void;
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
