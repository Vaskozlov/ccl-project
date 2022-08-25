#include <cerberus/text/text_iterator.hpp>

namespace cerb::text
{
    auto TextIterator::CommentSkipper::skip() -> bool
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

    auto TextIterator::CommentSkipper::isComment(u8string_view const &comment) const noexcept
        -> bool
    {
        auto text = text_iterator.getRemaining();
        return not comment.empty() && text.startsWith(comment);
    }

    auto TextIterator::CommentSkipper::skipSingleLine() -> void
    {
        text_iterator.skip(comment_tokens.single_line.size());

        auto chr = text_iterator.next();

        while (land(chr != 0, chr != U'\n')) {
            chr = text_iterator.next();
        }
    }

    auto TextIterator::CommentSkipper::skipMultiline() -> void
    {
        text_iterator.skip(comment_tokens.multiline_begin.size());
        const auto comment_begin = text_iterator;

        while (not isComment(comment_tokens.multiline_end) &&
               not isEoF(text_iterator.next())) {
            // empty loop
        }

        checkCommentTermination(comment_begin);
        text_iterator.skip(comment_tokens.multiline_end.size());
    }

    auto TextIterator::CommentSkipper::checkCommentTermination(
        const TextIterator &comment_begin) const -> void
    {
        if (isEoF(text_iterator.getCurrentChar())) {
            throwUnterminatedCommentError(comment_begin);
        }
    }

    auto TextIterator::CommentSkipper::throwUnterminatedCommentError(
        TextIterator const &comment_begin) const -> void
    {
        using namespace std::string_view_literals;

        text_iterator.throwException(CommentSkipperException(
            comment_begin.getLocation(),
            comment_begin.getWorkingLine(),
            u8"unterminated multiline comment"sv));
    }

    TextIterator::CommentSkipper::CommentSkipper(
        TextIterator &text_iterator_,
        const CommentTokens &comment_tokens_)
      : comment_tokens{ comment_tokens_ }, text_iterator{ text_iterator_ }
    {
        if (comment_tokens.multiline_begin.empty() != comment_tokens.multiline_end.empty()) {
            throw InvalidArgument("comment tokens must be either both empty or both non-empty");
        }
    }
}// namespace cerb::text
