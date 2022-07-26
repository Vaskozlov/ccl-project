#ifndef CERBERUS_PROJECT_TEXT_ITERATOR_HPP
#define CERBERUS_PROJECT_TEXT_ITERATOR_HPP

#include <cerberus/analysis/exception_accumulator.hpp>
#include <cerberus/text/location.hpp>
#include <cerberus/text/text_iterator_modules/comment_skipper.hpp>
#include <cerberus/text/text_iterator_modules/escaping_symbol.hpp>
#include <cerberus/text/text_iterator_modules/line_tracker.hpp>
#include <cerberus/text/text_iterator_modules/ts_tracker.hpp>

namespace cerb::text
{
    template<CharacterLiteral TextT, CharacterLiteral EscapingT>
    class TextIterator : public BasicTextIterator<TextT>
    {
    private:
        using Base = BasicTextIterator<TextT>;
        using CommentTokens = module::CommentTokens<TextT>;
        using CommentSkipper = module::CommentSkipper<TextT>;
        using LineTracker = module::LineTracker<TextT>;
        using EscapingSymbolizer = module::EscapingSymbolizer<TextT, EscapingT>;
        using ExtraSymbols = typename EscapingSymbolizer::extra_symbols_t;
        using TsTracker = module::TsTracker<TextT>;

    public:
        using ExceptionAccumulator = analysis::ExceptionAccumulator<TextIteratorException<TextT>>;

        CERBLIB_DECL auto getLocation() const -> const Location<> &
        {
            return location;
        }

        CERBLIB_DECL auto getLine() const -> size_t
        {
            return location.getLine();
        }

        CERBLIB_DECL auto getColumn() const -> size_t
        {
            return location.getColumn();
        }

        CERBLIB_DECL auto getFilename() const -> const StrView<char> &
        {
            return location.getFilename();
        }

        CERBLIB_DECL auto getWorkingLine() const -> const StrView<TextT> &
        {
            return line_tracker.get();
        }

        CERBLIB_DECL auto getTabsAndSpaces() const -> const Str<TextT> &
        {
            return ts_tracker.get();
        }

        [[nodiscard("You will not be allowed to get this char from getCurrentChar")]]// new line
        constexpr auto
            nextRawCharWithEscapingSymbols(const ExtraSymbols &extra_symbols = {})
                -> std::pair<bool, EscapingT>
        {
            auto chr = nextRawChar();

            if (chr == '\\') {
                return { true, module::doEscapeSymbolizing(*this, extra_symbols) };
            }

            return { false, chr };
        }

        constexpr auto nextRawChar() -> TextT override
        {
            auto chr = Base::basicNextRawChar();
            updateModules(chr);
            return chr;
        }

        constexpr auto skipCommentsAndLayout() -> void
        {
            auto comment_skipper = CommentSkipper{ *this, comment_tokens };

            do {
                Base::moveToCleanChar();
            } while (comment_skipper.skip());
        }

        template<Exception T>
        constexpr auto throwException(T &&exception) -> void
        {
            addError(exceptions, std::forward<T>(exception));
        }

        template<Exception T>
        constexpr auto throwWarning(T &&exception) -> void
        {
            addWarning(exceptions, std::forward<T>(exception));
        }

        auto operator=(const TextIterator &) -> TextIterator & = default;
        auto operator=(TextIterator &&) noexcept -> TextIterator & = default;

        TextIterator(const TextIterator &) = default;
        TextIterator(TextIterator &&) noexcept = default;

        constexpr explicit TextIterator(
            StrView<TextT>
                input,
            ExceptionAccumulator *exceptions_ = nullptr,
            CommentTokens comment_tokens_ = {},
            string_view filename = {})
          : Base(input), comment_tokens(comment_tokens_), location(filename),
            exceptions(exceptions_)
        {}

        ~TextIterator() = default;

    private:
        constexpr auto updateModules(TextT chr) -> void
        {
            location.next(chr);
            line_tracker.next(chr);
            ts_tracker.next(chr);
        }

        LineTracker line_tracker{ Base::getRemaining() };
        CommentTokens comment_tokens{};
        Location<char> location{};
        TsTracker ts_tracker{};
        ExceptionAccumulator *exceptions{ nullptr };
    };
}// namespace cerb::text

#endif /* CERBERUS_PROJECT_TEXT_ITERATOR_HPP */
