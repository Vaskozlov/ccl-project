#ifndef CERBERUS_PROJECT_TEXT_ITERATOR_HPP
#define CERBERUS_PROJECT_TEXT_ITERATOR_HPP

#include <cerberus/analysis/exception.hpp>
#include <cerberus/text/basic_text_iterator.hpp>
#include <cerberus/text/location.hpp>
#include <cerberus/text/text_iterator_modules/comment_skipper.hpp>
#include <cerberus/text/text_iterator_modules/escaping_symbol.hpp>
#include <cerberus/text/text_iterator_modules/line_tracker.hpp>

namespace cerb::text
{
    template<CharacterLiteral TextT, CharacterLiteral EscapingT>
    class TextIterator : public BasicTextIterator<TextT>
    {
    private:
        using Base = BasicTextIterator<TextT>;
        using CommentSkipper = module::CommentSkipper<TextT>;
        using LineTracker = module::LineTracker<TextT>;
        using EscapingSymbolizer = module::EscapingSymbolizer<TextT, EscapingT>;
        using ExtraSymbols = typename EscapingSymbolizer::extra_symbols_t;
        using ExceptionAccumulator = analysis::ExceptionAccumulator<TextIteratorException<TextT>>;

    public:
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

        [[nodiscard("You will not be allowed to get this char from getCurrentChar")]]// new line
        constexpr auto
            nextRawCharWithEscapingSymbols(const ExtraSymbols &extra_symbols = {})
                -> std::pair<bool, EscapingT>
        {
            auto chr = nextRawChar();

            if (chr == '\\') {
                escaping_symbolizer.setExtraSymbols(extra_symbols);
                return { true, escaping_symbolizer.match(*this) };
            }

            return { false, chr };
        }

        constexpr auto nextRawChar() -> TextT override
        {
            auto chr = Base::basicNextRawChar();
            location.next(chr);
            line_tracker.next(*this);
            return chr;
        }

        constexpr auto skipCommentsAndLayout() -> void
        {
            do {
                Base::moveToCleanChar();
            } while (comment_skipper.skip(*this));
        }

        template<Exception T>
        constexpr auto throwException(T &&exception) -> void
        {
            addError(exceptions, std::forward<T>(exception));
        }

        auto operator=(const TextIterator &) -> TextIterator & = default;
        auto operator=(TextIterator &&) noexcept -> TextIterator & = default;

        TextIterator(const TextIterator &) = default;
        TextIterator(TextIterator &&) noexcept = default;

        constexpr explicit TextIterator(
            StrView<TextT>
                input,
            ExceptionAccumulator *exceptions_ = nullptr,
            CommentSkipper comment_skipper_ = {},
            string_view filename = {})
          : Base(input), comment_skipper(comment_skipper_), location(filename),
            exceptions(exceptions_)
        {}

        ~TextIterator() = default;

    private:
        LineTracker line_tracker{ *this };
        CommentSkipper comment_skipper{};
        EscapingSymbolizer escaping_symbolizer{ {} };
        Location<char> location{};
        ExceptionAccumulator *exceptions{ nullptr };
    };
}// namespace cerb::text

#endif /* CERBERUS_PROJECT_TEXT_ITERATOR_HPP */
