#ifndef CCL_PROJECT_TEXT_ITERATOR_HPP
#define CCL_PROJECT_TEXT_ITERATOR_HPP

#include <ccl/analysis/exception_accumulator.hpp>
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
        std::u8string single_line{};
        std::u8string multiline_begin{};
        std::u8string multiline_end{};
    };

    CCL_EXCEPTION(CommentSkipperException, TextIteratorException);
    CCL_EXCEPTION(NotationEscapingSymbolizerException, TextIteratorException);

    class TextIterator : public BasicTextIterator
    {
    private:
        using Base = BasicTextIterator;
        using extra_symbols_t = std::basic_string<Pair<char32_t, char32_t>>;

    public:
        using ExceptionAccumulator = analysis::ExceptionAccumulator<TextIteratorException>;

        class CommentSkipper;
        class EscapingSymbolizer;
        class NotationEscapingSymbolizer;

        explicit TextIterator(
            u8string_view input, ExceptionAccumulator *exception_accumulator_ = {},
            const CommentTokens &comment_tokens_ = {}, u8string_view filename = {})
          : Base(input), location(filename), line_tracker(input), comment_tokens(comment_tokens_),
            exception_accumulator(exception_accumulator_)
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

        [[nodiscard]] auto getFilename() const noexcept -> const u8string_view &
        {
            return location.getFilename();
        }

        [[nodiscard]] auto getWorkingLine() const noexcept -> const u8string_view &
        {
            return line_tracker.get();
        }

        [[nodiscard]] auto getTabsAndSpaces() const noexcept -> const std::u32string &
        {
            return ts_tracker.get();
        }

        auto nextRawCharWithEscapingSymbols(const extra_symbols_t &extra_symbols = {})
            -> Pair<bool, char32_t>;

        auto onMove(char8_t chr) -> void override;
        auto onCharacter(char32_t chr) -> void override;
        auto onUtfError(char8_t chr) -> void override;

        auto skipComments() -> bool;
        auto skipCommentsAndLayout() -> void;

        template<Exception T>
        auto throwException(T &&exception) -> void
        {
            addError(exception_accumulator, std::forward<T>(exception));
        }

        template<Exception T, typename... Ts>
        auto throwException(Ts &&...args) -> void
        {
            addError(
                exception_accumulator,
                T(getLocation(), getWorkingLine(), std::forward<Ts>(args)...));
        }

        template<Exception T>
        auto throwWarning(T &&exception) -> void
        {
            addWarning(exception_accumulator, std::forward<T>(exception));
        }

        template<Exception T, typename... Ts>
        auto throwWarning(Ts &&...args) -> void
        {
            addWarning(
                exception_accumulator,
                T(getLocation(), getWorkingLine(), std::forward<Ts>(args)...));
        }

    private:
        Location location{};
        module::TsTracker ts_tracker{};
        module::LineTracker line_tracker;
        CommentTokens comment_tokens{};
        ExceptionAccumulator *exception_accumulator{};
    };

    class TextIterator::CommentSkipper
    {
    public:
        auto skip() -> bool;

        CommentSkipper(TextIterator &text_iterator_, const CommentTokens &comment_tokens_);

    private:
        [[nodiscard]] auto isComment(const u8string_view &comment) const noexcept -> bool;

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

        [[nodiscard]] auto match() -> char32_t;// TODO: rename function

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

        [[nodiscard]] auto createSuggestionNotEnoughChars(u16 chars_count) const -> std::u8string;

        auto insertExtraZerosToNotEnoughMessage(u16 chars_count, std::u8string &message) const
            -> void;

        TextIterator &text_iterator;
        char32_t result{};
        u16 max_times;
        u16 notation_power;
        bool need_all_chars;
    };
}// namespace ccl::text

#endif /* CCL_PROJECT_TEXT_ITERATOR_HPP */
