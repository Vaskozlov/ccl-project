#ifndef CERBERUS_PROJECT_COMMENT_SKIPPER_HPP
#define CERBERUS_PROJECT_COMMENT_SKIPPER_HPP

#include <cerberus/text/basic_text_iterator.hpp>
#include <cerberus/text/text_iterator_modules/exception.hpp>

namespace cerb::text
{
    template<CharacterLiteral CharT>
    class TextIterator;

    namespace module
    {
        template<CharacterLiteral CharT>
        CERBLIB_EXCEPTION(CommentSkipperException, TextIteratorException<CharT>);

        template<CharacterLiteral CharT>
        class CommentSkipper
        {
        public:
            using StrView = BasicStringView<CharT>;

            constexpr auto skip(TextIterator<CharT> &text_iterator_) -> bool
            {
                text_iterator = &text_iterator_;

                if (isComment(single_line)) {
                    skipSingleLine();
                    return true;
                }

                if (isComment(multiline_begin)) {
                    skipMultiline();
                    return true;
                }

                return {};
            }

            CommentSkipper() = default;

            constexpr CommentSkipper(
                StrView single_line_,
                StrView multiline_begin_,
                StrView multiline_end_)
              : single_line(single_line_), multiline_begin(multiline_begin_),
                multiline_end(multiline_end_)
            {
                if (land(not multiline_begin.empty(), multiline_end.empty())) {
                    throw LogicError{
                        "multiline comment end can't be empty, when it's begin is not"
                    };
                }
            }

        private:
            CERBLIB_DECL auto isComment(const StrView &comment) const -> bool
            {
                auto text = text_iterator->getRemainingFuture(1);
                return not comment.empty() && (text.substr(0, comment.size()) == comment);
            }

            constexpr auto skipSingleLine() -> void
            {
                text_iterator->rawSkip(single_line.size());

                CharT chr = text_iterator->nextRawChar();

                for (; land(chr != 0, chr != '\n'); chr = text_iterator->nextRawChar()) {
                    // empty loop
                }
            }

            constexpr auto skipMultiline() -> void
            {
                const auto *comment_begin = text_iterator;

                text_iterator->rawSkip(multiline_begin.size());

                while (not isComment(multiline_end) && not isEoF(text_iterator->getCurrentChar())) {
                    text_iterator->nextRawChar();
                }

                checkCommentTermination(comment_begin);
                text_iterator->rawSkip(multiline_end.size());
            }

            constexpr auto checkCommentTermination(const TextIterator<CharT> *comment_begin) const
                -> void
            {
                if (isEoF(text_iterator->getCurrentChar())) {
                    throw CommentSkipperException<CharT>(
                        *comment_begin, "unterminated multiline comment");
                }
            }

            StrView single_line{};
            StrView multiline_begin{};
            StrView multiline_end{};
            TextIterator<CharT> *text_iterator{ nullptr };
        };
    }// namespace module
}// namespace cerb::text

#endif /* CERBERUS_PROJECT_COMMENT_SKIPPER_HPP */
