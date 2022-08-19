#include <cerberus/lex/analysis_shared.hpp>
#include <cerberus/lex/dot_item/sequence.hpp>

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
        return not comment.empty() && text.startsWith(comment);
    }

    auto AnalysisShared::getSpecialToken(text::TextIterator &text_iterator) const
        -> std::optional<Token>
    {
        auto text = text_iterator.getFutureRemaining(1);
        auto terminal_match_result = terminals.match(text);

        if (terminal_match_result.has_value()) {
            return constructTerminalToken(text_iterator, text, *terminal_match_result);
        }

        auto string_or_char_elem =
            std::ranges::find_if(strings_and_chars, [text](const String &elem) {
                return text.startsWith(elem.str_begin);
            });

        if (string_or_char_elem != strings_and_chars.end()) {
            return constructStringToken(text_iterator, *string_or_char_elem);
        }

        return std::nullopt;
    }

    auto AnalysisShared::constructTerminalToken(
        text::TextIterator &text_iterator, const u8string_view &remaining_text,
        std::pair<std::u8string, size_t> &terminal_match) -> Token
    {
        auto &[token_value, id] = terminal_match;
        auto repr = remaining_text.substr(0, token_value.size());
        auto token_attributes = TokenAttributes{ text_iterator };

        text_iterator.rawSkip(token_value.size());

        return { std::move(token_attributes), repr, id, std::move(token_value) };
    }

    auto AnalysisShared::constructStringToken(
        text::TextIterator &text_iterator, const String &string_elem) -> Token
    {
        text_iterator.nextRawChar();

        auto empty_shared = AnalysisShared{};
        auto token_attributes = TokenAttributes{ text_iterator };
        const auto *repr_begin = text_iterator.getCarriage();
        auto &[str_begin, str_end, id, is_character, is_multiline] = string_elem;

        auto sequence = dot_item::Sequence(
            { .multiline = is_multiline, .no_escaping_symbols = is_multiline }, str_begin, str_end,
            text_iterator, empty_shared);

        auto &sequence_string = sequence.getRef();

        if (string_elem.is_character && sequence_string.empty()) {
            text_iterator.throwException<LexicalAnalysisException>(u8"empty character definition");
        }

        if (string_elem.is_character && sequence_string.size() > 1 &&
            utf8::utfSize(sequence_string[0]) != sequence_string.size()) {
            text_iterator.throwException<LexicalAnalysisException>(
                u8"character definition must be a single character");
        }

        return { std::move(token_attributes),
                 { repr_begin, text_iterator.getRemainingAsCarriage() },
                 id,
                 std::move(sequence_string) };
    }

    auto AnalysisShared::isStringOrChar(const u8string_view &text) const -> bool
    {
        return std::ranges::any_of(strings_and_chars, [text](const String &elem) {
            return text.startsWith(elem.str_begin);
        });
    }
}// namespace cerb::lex
