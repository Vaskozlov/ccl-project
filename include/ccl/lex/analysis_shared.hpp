#ifndef CCL_PROJECT_ANALYSIS_SHARED_HPP
#define CCL_PROJECT_ANALYSIS_SHARED_HPP

#include <ccl/lex/token.hpp>
#include <ccl/raw_string_matcher.hpp>
#include <ccl/text/text_iterator.hpp>
#include <optional>
#include <utility>
#include <vector>

namespace ccl::lex
{
    struct String
    {
        CCL_PERFECT_FORWARDING_2(T1, std::string, T2, std::string)
        String(T1 &&str_begin_, T2 &&str_end_, size_t id_, bool is_character_, bool is_multiline_)
          : str_begin(std::forward<T1>(str_begin_)), str_end(std::forward<T2>(str_end_)), id(id_),
            is_character(is_character_), is_multiline(is_multiline_)
        {}

        CCL_PERFECT_FORWARDING(T, std::string)
        String(T &&str_begin_, size_t id_, bool is_character_, bool is_multiline_)
          : str_begin(std::forward<T>(str_begin_)), str_end(str_begin), id(id_),
            is_character(is_character_), is_multiline(is_multiline_)
        {}

        std::string str_begin{};
        std::string str_end{};
        size_t id{};
        bool is_character{};
        bool is_multiline{};
    };

    struct AnalysisShared
    {
        using CommentTokens = text::CommentTokens;

        [[nodiscard]] auto isTerminal(const string_view &text) const -> bool
        {
            return special_tokens.matches(text);
        }

        [[nodiscard]] auto isComment(const string_view &text) const noexcept -> bool;
        [[nodiscard]] auto isNextCharNotForScanning(const text::TextIterator &text_iterator) const
            -> bool;

        [[nodiscard]] auto getSpecialToken(text::TextIterator &text_iterator) const
            -> std::optional<Token>;

        [[nodiscard]] auto isStringOrChar(const string_view &text) const noexcept -> bool;

    private:
        [[nodiscard]] static auto
            basicIsComment(const string_view &text, const string_view &comment) noexcept -> bool;

        // always contains token, but optional for RVO
        [[nodiscard]] static auto constructTerminalToken(
            text::TextIterator &text_iterator,
            const string_view &remaining_text,
            std::pair<std::string, size_t> &special_symbols_matcher) -> std::optional<Token>;

        // always contains token, but optional for RVO
        [[nodiscard]] static auto
            constructStringToken(text::TextIterator &text_iterator, const String &string_elem)
                -> std::optional<Token>;

        static auto isEmptyCharacterDefinition(bool is_character, const std::string &string_value)
            -> bool;

        static auto
            isMoreThanOneCharacterInCharacter(bool is_character, const std::string &string_value)
                -> bool;

    public:
        CommentTokens comment_tokens{};
        boost::container::small_vector<String, 4> strings_and_chars{};
        RawStringMatcher special_tokens{};
    };
}// namespace ccl::lex

#endif /* CCL_PROJECT_ANALYSIS_SHARED_HPP */
