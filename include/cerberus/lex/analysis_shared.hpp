#ifndef CERBERUS_PROJECT_ANALYSIS_SHARED_HPP
#define CERBERUS_PROJECT_ANALYSIS_SHARED_HPP

#include <cerberus/lex/token.hpp>
#include <cerberus/raw_string_matcher.hpp>
#include <cerberus/text/text_iterator.hpp>
#include <optional>
#include <utility>
#include <vector>

namespace cerb::lex
{
    struct String
    {
        String(
            std::u8string str_begin_, std::u8string str_end_, size_t id_, bool is_character_,
            bool is_multiline_)
          : str_begin(std::move(str_begin_)), str_end(std::move(str_end_)), id(id_),
            is_character(is_character_), is_multiline(is_multiline_)
        {}

        String(const std::u8string &str_begin_, size_t id_, bool is_character_, bool is_multiline_)
          : String(str_begin_, str_begin_, id_, is_character_, is_multiline_)
        {}

        std::u8string str_begin;
        std::u8string str_end{};
        size_t id;
        bool is_character;
        bool is_multiline;
    };

    struct AnalysisShared
    {
        using CommentTokens = text::CommentTokens;

        [[nodiscard]] auto isTerminal(const u8string_view &text) const -> bool
        {
            return terminals.matches(text);
        }

        [[nodiscard]] auto isComment(const u8string_view &text) const -> bool;
        [[nodiscard]] auto isNextCharNotForScanning(const text::TextIterator &text_iterator) const
            -> bool;

        [[nodiscard]] auto getSpecialToken(text::TextIterator &text_iterator) const
            -> std::optional<Token>;

        [[nodiscard]] auto isStringOrChar(const u8string_view &text) const -> bool;

    private:
        [[nodiscard]] static auto
            basicIsComment(const u8string_view &text, const u8string_view &comment) -> bool;

        [[nodiscard]] static auto constructTerminalToken(
            text::TextIterator &text_iterator,
            const u8string_view &remaining_text,
            std::pair<std::u8string, size_t> &terminal_match) -> Token;

        [[nodiscard]] static auto
            constructStringToken(text::TextIterator &text_iterator, const String &string_elem)
                -> Token;

    public:
        CommentTokens comment_tokens{};
        std::vector<String> strings_and_chars{};
        RawStringMatcher terminals{};
    };
}// namespace cerb::lex

#endif /* CERBERUS_PROJECT_ANALYSIS_SHARED_HPP */
