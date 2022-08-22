#ifndef CERBERUS_PROJECT_TEXT_ITERATOR_HPP
#define CERBERUS_PROJECT_TEXT_ITERATOR_HPP

#include <cerberus/analysis/exception_accumulator.hpp>
#include <cerberus/pair.hpp>
#include <cerberus/text/iterator_exception.hpp>
#include <cerberus/text/location.hpp>
#include <cerberus/text/text_iterator_modules/line_tracker.hpp>
#include <cerberus/text/text_iterator_modules/ts_tracker.hpp>
#include <utility>

namespace cerb::text
{
    struct CommentTokens
    {
        std::u8string single_line{};
        std::u8string multiline_begin{};
        std::u8string multiline_end{};
    };

    CERBLIB_EXCEPTION(CommentSkipperException, TextIteratorException);
    CERBLIB_EXCEPTION(NotationEscapingSymbolizerException, TextIteratorException);

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

        [[nodiscard]] static auto
            doEscapeSymbolizing(TextIterator &text_iterator, const extra_symbols_t &extra_symbols_)
                -> char32_t;

        [[nodiscard]] static auto calculateNotationEscapeSymbol(
            TextIterator &text_iterator, u16 max_times, u16 notation_power, bool need_all_chars)
            -> char32_t;

        [[nodiscard]] auto getLocation() const -> const Location &
        {
            return location;
        }

        [[nodiscard]] auto getLine() const -> size_t
        {
            return location.getLine();
        }

        [[nodiscard]] auto getColumn() const -> size_t
        {
            return location.getColumn();
        }

        [[nodiscard]] auto getRealColumn() const -> size_t
        {
            return location.getRealColumn();
        }

        [[nodiscard]] auto getFilename() const -> const u8string_view &
        {
            return location.getFilename();
        }

        [[nodiscard]] auto getWorkingLine() const -> const u8string_view &
        {
            return line_tracker.get();
        }

        [[nodiscard]] auto getTabsAndSpaces() const -> const std::u32string &
        {
            return ts_tracker.get();
        }

        auto nextRawCharWithEscapingSymbols(const extra_symbols_t &extra_symbols = {})
            -> Pair<bool, char32_t>;

        auto onMove(char8_t chr) -> void override;
        auto onCharacter(char32_t chr) -> void override;
        auto onUtfError(char8_t chr) -> void override;

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

        auto operator=(const TextIterator &) -> TextIterator & = default;
        auto operator=(TextIterator &&) noexcept -> TextIterator & = default;

        TextIterator(const TextIterator &) = default;
        TextIterator(TextIterator &&) noexcept = default;

        explicit TextIterator(
            u8string_view input, ExceptionAccumulator *exception_accumulator_ = {},
            CommentTokens comment_tokens_ = {}, u8string_view filename = {})
          : Base(input), location(filename), line_tracker(input), comment_tokens(comment_tokens_),
            exception_accumulator(exception_accumulator_)
        {}

        ~TextIterator() override = default;

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
        [[nodiscard]] auto isComment(const u8string_view &comment) const -> bool;

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
        [[nodiscard]] auto getExtraSymbols() const -> const extra_symbols_t &
        {
            return extra_symbols;
        }

        [[nodiscard]] auto match() -> char32_t;

        auto operator=(EscapingSymbolizer &&) -> void = delete;
        auto operator=(const EscapingSymbolizer &) -> void = delete;

        EscapingSymbolizer() = delete;
        EscapingSymbolizer(EscapingSymbolizer &&) = delete;
        EscapingSymbolizer(const EscapingSymbolizer &) = delete;

        explicit EscapingSymbolizer(TextIterator &text_iterator_, extra_symbols_t extra_symbols_)
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
        CERBLIB_DECL auto get() const -> char32_t
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
            bool need_all_chars_);

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
}// namespace cerb::text

#endif /* CERBERUS_PROJECT_TEXT_ITERATOR_HPP */
