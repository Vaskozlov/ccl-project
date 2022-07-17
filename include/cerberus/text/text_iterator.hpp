#ifndef CERBERUS_PROJECT_TEXT_ITERATOR_HPP
#define CERBERUS_PROJECT_TEXT_ITERATOR_HPP

#include <cerberus/text/basic_text_iterator.hpp>
#include <cerberus/text/location.hpp>
#include <cerberus/text/text_iterator_modules/comment_skipper.hpp>
#include <cerberus/text/text_iterator_modules/escaping_symbol.hpp>
#include <cerberus/text/text_iterator_modules/line_tracker.hpp>

namespace cerb::text
{
    template<CharacterLiteral CharT>
    class TextIterator : public BasicTextIterator<CharT>
    {
        using Base = BasicTextIterator<CharT>;
        using extra_symbols_t = std::initializer_list<std::pair<CharT, CharT>>;

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

        CERBLIB_DECL auto getFilename() const -> const string_view &
        {
            return location.getFilename();
        }

        CERBLIB_DECL auto getWorkingLine() const -> const BasicStringView<CharT> &
        {
            return line_tracker.get();
        }

        constexpr auto nextRawCharWithEscapingSymbols(const extra_symbols_t &extra_symbols = {})
            -> CharT
        {
            auto chr = nextRawChar();

            if (chr == '\\') {
                auto escaping_symbol = module::EscapingSymbolizer{ *this, extra_symbols };
                return escaping_symbol.match();
            }

            return chr;
        }

        constexpr auto nextRawChar() -> CharT override
        {
            auto chr = Base::basicNextRawChar();
            location.next(chr);
            line_tracker.next();
            return chr;
        }

        constexpr auto skipCommentsAndLayout() -> void
        {
            do {
                Base::moveToCleanChar();
            } while (comment_skipper.skip(*this));
        }

        auto operator=(const TextIterator &) -> TextIterator & = default;
        auto operator=(TextIterator &&) noexcept -> TextIterator & = default;

        TextIterator(const TextIterator &) = default;
        TextIterator(TextIterator &&) noexcept = default;

        constexpr explicit TextIterator(
            BasicStringView<CharT> input, module::CommentSkipper<CharT> comment_skipper_ = {},
            string_view filename = {})
          : Base{ input }, comment_skipper{ comment_skipper_ }, location{ filename }
        {}

        ~TextIterator() = default;

    private:
        module::LineTracker<CharT> line_tracker{ *this };
        module::CommentSkipper<CharT> comment_skipper{};
        Location<char> location{};
    };
}// namespace cerb::text

#endif /* CERBERUS_PROJECT_TEXT_ITERATOR_HPP */
