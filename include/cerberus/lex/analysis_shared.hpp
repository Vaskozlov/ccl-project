#ifndef CERBERUS_PROJECT_ANALYSIS_SHARED_HPP
#define CERBERUS_PROJECT_ANALYSIS_SHARED_HPP

#include <cerberus/string_map.hpp>
#include <cerberus/text/text_iterator.hpp>
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

    private:
        [[nodiscard]] static auto
            basicIsComment(const u8string_view &text, const u8string_view &comment) -> bool;

    public:
        [[nodiscard]] auto isStringOrChar(const u8string_view &text) const -> bool;

        CommentTokens comment_tokens{};
        std::vector<String> strings_and_chars{};
        StringMap<char8_t, size_t> terminals{};
    };
}// namespace cerb::lex

#endif /* CERBERUS_PROJECT_ANALYSIS_SHARED_HPP */
