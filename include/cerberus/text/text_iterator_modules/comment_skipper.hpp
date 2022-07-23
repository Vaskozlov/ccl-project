#ifndef CERBERUS_PROJECT_COMMENT_SKIPPER_HPP
#define CERBERUS_PROJECT_COMMENT_SKIPPER_HPP

#include <cerberus/text/basic_text_iterator.hpp>
#include <cerberus/text/text_iterator_modules/exception.hpp>

namespace cerb::text::module
{
    template<CharacterLiteral CharT>
    CERBLIB_EXCEPTION(CommentSkipperException, TextIteratorException<CharT>);

    template<CharacterLiteral CharT>
    struct CommentTokens
    {
        StrView<CharT> single_line{};
        StrView<CharT> multiline_begin{};
        StrView<CharT> multiline_end{};
    };

    template<CharacterLiteral CharT>
    class CommentSkipper
    {
    public:
        constexpr auto skip() -> bool
        {
            if (isComment(comment_tokens.single_line)) {
                skipSingleLine();
                return true;
            }

            if (isComment(comment_tokens.multiline_begin)) {
                skipMultiline();
                return true;
            }

            return false;
        }

        CommentSkipper() = default;

        constexpr CommentSkipper(
            TextIterator<CharT> &text_iterator_,
            const CommentTokens<CharT> &comment_tokens_)
          : comment_tokens(comment_tokens_), text_iterator(text_iterator_)
        {
            if (land(
                    not comment_tokens.multiline_begin.empty(),
                    comment_tokens.multiline_end.empty())) {
                throw LogicError("multiline comment end can't be empty, when it's begin is not");
            }
        }

    private:
        CERBLIB_DECL auto isComment(const StrView<CharT> &comment) const -> bool
        {
            auto text = text_iterator.getRemainingFuture(1);
            return not comment.empty() && (text.substr(0, comment.size()) == comment);
        }

        constexpr auto skipSingleLine() -> void
        {
            text_iterator.rawSkip(comment_tokens.single_line.size());

            CharT chr = text_iterator.nextRawChar();

            while (land(chr != 0, chr != '\n')) {
                chr = text_iterator.nextRawChar();
            }
        }

        constexpr auto skipMultiline() -> void
        {
            text_iterator.rawSkip(comment_tokens.multiline_begin.size());
            const auto comment_begin = text_iterator;

            while (not isComment(comment_tokens.multiline_end) &&
                   not isEoF(text_iterator.nextRawChar())) {
                // empty loop
            }

            checkCommentTermination(comment_begin);
            text_iterator.rawSkip(comment_tokens.multiline_end.size());
        }

        constexpr auto checkCommentTermination(const TextIterator<CharT> &comment_begin) const
            -> void
        {
            if (isEoF(text_iterator.getCurrentChar())) {
                throwUnterminatedCommentError(comment_begin);
            }
        }

        constexpr auto throwUnterminatedCommentError(const TextIterator<CharT> &comment_begin) const
            -> void
        {
            auto exception =
                CommentSkipperException<CharT>(comment_begin, "unterminated multiline comment");

            text_iterator.throwException(std::move(exception));
        }

        const CommentTokens<CharT> &comment_tokens;// MAYBE: copy comment tokens into StrViews
        TextIterator<CharT> &text_iterator;
    };
}// namespace cerb::text::module

#endif /* CERBERUS_PROJECT_COMMENT_SKIPPER_HPP */
