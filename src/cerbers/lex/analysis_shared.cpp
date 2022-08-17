#include <cerberus/lex/analysis_shared.hpp>

namespace cerb::lex
{
    auto AnalysisShared::isNextCharNotForScanning(const text::TextIterator &text_iterator) const
        -> bool
    {
        auto text = text_iterator.getFutureRemaining(1);

        return isComment(text) || isTerminal(text) || isStringOrChar(text);
    }

    auto AnalysisShared::isComment(const u8string_view &text) const -> bool
    {
        return basicIsComment(text, comment_tokens.single_line) ||
               basicIsComment(text, comment_tokens.multiline_begin) ||
               basicIsComment(text, comment_tokens.multiline_end);
    }

    auto AnalysisShared::basicIsComment(const u8string_view &text, const u8string_view &comment)
        -> bool
    {
        return not comment.empty() && text.substr(0, comment.size()) == comment;
    }

    auto AnalysisShared::isStringOrChar(const u8string_view &text) const -> bool
    {
        return std::ranges::any_of(strings_and_chars, [text](const auto &elem) {
            return text.substr(0, elem.str_begin.size()) == elem.str_begin;
        });
    }
}// namespace cerb::lex
