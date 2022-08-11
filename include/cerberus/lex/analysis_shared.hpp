#ifndef CERBERUS_PROJECT_ANALYSIS_SHARED_HPP
#define CERBERUS_PROJECT_ANALYSIS_SHARED_HPP

#include <cerberus/string_map.hpp>
#include <cerberus/text/text_iterator.hpp>
#include <vector>

namespace cerb::lex
{
    struct String
    {
        String(const std::u8string &str_, size_t id_, bool is_character_, bool is_multiline_)
          : str(str_), id(id_), is_character(is_character_), is_multiline(is_multiline_)
        {}

        String(std::u8string &&str_, size_t id_, bool is_character_, bool is_multiline_)
          : str(std::move(str_)), id(id_), is_character(is_character_), is_multiline(is_multiline_)
        {}

        std::u8string str;
        size_t id;
        bool is_character;
        bool is_multiline;
    };

    struct AnalysisShared
    {
        using CommentTokens = text::CommentTokens;

        [[nodiscard]] auto isNextCharNotForScanning(const text::TextIterator &text_iterator) const
            -> bool
        {
            auto text = text_iterator.getRemainingFutureAfterSymbols(1);

            return isComment(text) || isTerminal(text) || isStringOrChar(text);
        }

        [[nodiscard]] auto isTerminal(const u8string_view &text) const -> bool
        {
            return terminals.matches(text).success;
        }

        [[nodiscard]] auto isComment(const u8string_view &text) const -> bool
        {
            return basicIsComment(text, comment_tokens.single_line) ||
                   basicIsComment(text, comment_tokens.multiline_begin) ||
                   basicIsComment(text, comment_tokens.multiline_end);
        }

    private:
        [[nodiscard]] static auto
            basicIsComment(const u8string_view &text, const u8string_view &comment) -> bool
        {
            return not comment.empty() && text.substr(0, comment.size()) == comment;
        }

    public:
        [[nodiscard]] auto isStringOrChar(const u8string_view &text) const -> bool
        {
            return std::ranges::any_of(strings_and_chars, [text](const auto &elem) {
                return text.substr(0, elem.str.size()) == elem.str;
            });
        }

        CommentTokens comment_tokens{};
        std::vector<String> strings_and_chars{};
        StringMap<char8_t, size_t> terminals{};
    };
}// namespace cerb::lex

#endif /* CERBERUS_PROJECT_ANALYSIS_SHARED_HPP */
