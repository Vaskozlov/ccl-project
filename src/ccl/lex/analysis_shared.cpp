#include <ccl/lex/analysis_shared.hpp>
#include <ccl/lex/dot_item/sequence.hpp>

namespace ccl::lex
{
    using namespace string_view_literals;

    static auto EmptyShared = AnalysisShared{};// NOLINT

    auto AnalysisShared::isNextCharNotForScanning(const text::TextIterator &text_iterator) const
        -> bool
    {
        auto text = text_iterator.getFutureRemaining(1);

        return isLayoutOrEoF(text_iterator.getNextCarriageValue()) || isComment(text) ||
               isTerminal(text) || isStringOrChar(text);
    }

    auto AnalysisShared::isComment(const string_view &text) const noexcept -> bool
    {
        return basicIsComment(text, comment_tokens.single_line) ||
               basicIsComment(text, comment_tokens.multiline_begin) ||
               basicIsComment(text, comment_tokens.multiline_end);
    }

    auto
        AnalysisShared::basicIsComment(const string_view &text, const string_view &comment) noexcept
        -> bool
    {
        return not comment.empty() && text.startsWith(comment);
    }

    auto AnalysisShared::getSpecialToken(text::TextIterator &text_iterator) const
        -> std::optional<Token>
    {
        auto text = text_iterator.getFutureRemaining(1);
        auto special_match_result = special_tokens.match(text);

        if (special_match_result.has_value()) {
            return constructTerminalToken(text_iterator, text, *special_match_result);
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
        text::TextIterator &text_iterator, const string_view &remaining_text,
        std::pair<std::string, size_t> &special_symbols_matcher) -> std::optional<Token>
    {
        auto &[token_value, id] = special_symbols_matcher;
        auto repr = remaining_text.substr(0, token_value.size());
        auto token_attributes = TokenAttributes{ text_iterator };

        text_iterator.skip(token_value.size());

        return std::optional<Token>(
            std::in_place, std::move(token_attributes), repr, id, std::move(token_value));
    }

    auto AnalysisShared::constructStringToken(
        text::TextIterator &text_iterator, const String &string_elem) -> std::optional<Token>
    {
        text_iterator.next();

        auto repr_begin = text_iterator.getCarriage();
        auto token_attributes = TokenAttributes{ text_iterator };
        const auto &[str_begin, str_end, id, is_character, is_multiline] = string_elem;

        auto sequence = dot_item::Sequence(
            { .multiline = is_multiline, .no_escaping_symbols = is_multiline }, str_begin, str_end,
            text_iterator, EmptyShared);

        auto &sequence_value = sequence.getByRef();

        if (isEmptyCharacterDefinition(is_character, sequence_value)) {
            return std::nullopt;
        }

        if (isMoreThanOneCharacterInCharacter(is_character, sequence_value)) {
            return std::nullopt;
        }

        return std::optional<Token>(
            std::in_place, std::move(token_attributes),
            string_view{ repr_begin, text_iterator.getRemainingAsCarriage() }, id,
            std::move(sequence_value));
    }

    auto AnalysisShared::isStringOrChar(const string_view &text) const noexcept -> bool
    {
        return std::ranges::any_of(strings_and_chars, [text](const String &elem) {
            return text.startsWith(elem.str_begin);
        });
    }

    auto AnalysisShared::isEmptyCharacterDefinition(
        bool is_character, const std::string &string_value) -> bool
    {
        return is_character && string_value.empty();
    }

    auto AnalysisShared::isMoreThanOneCharacterInCharacter(
        bool is_character, const std::string &string_value) -> bool
    {
        return is_character && string_value.size() > 1 &&
               utf8::utfSize(string_value[0]) != string_value.size();
    }
}// namespace ccl::lex
