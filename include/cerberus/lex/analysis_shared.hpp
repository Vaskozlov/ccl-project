#ifndef CERBERUS_PROJECT_ANALYSIS_SHARED_HPP
#define CERBERUS_PROJECT_ANALYSIS_SHARED_HPP

#include <cerberus/lex/typedefs.hpp>
#include <cerberus/string_map.hpp>
#include <cerberus/text/text_iterator.hpp>
#include <vector>

namespace cerb::lex
{
    template<CharacterLiteral CharT>
    struct String
    {
        String(const Str<CharT> &str_, size_t id_, bool is_character_, bool is_multiline_)
          : str(str_), id(id_), is_character(is_character_), is_multiline(is_multiline_)
        {}

        constexpr String(Str<CharT> &&str_, size_t id_, bool is_character_, bool is_multiline_)
          : str(std::move(str_)), id(id_), is_character(is_character_), is_multiline(is_multiline_)
        {}

        Str<CharT> str;
        size_t id;
        bool is_character;
        bool is_multiline;
    };

    template<CharacterLiteral CharT>
    struct AnalysisShared
    {
        using CommentTokens = text::module::CommentTokens<CharT>;

        CERBLIB_DECL auto
            isNextCharNotForScanning(const text::TextIterator<CharT> &text_iterator) const -> bool
        {
            auto text = text_iterator.getRemainingFuture(1);

            return isComment(text) || isTerminal(text) || isStringOrChar(text);
        }

        CERBLIB_DECL auto isTerminal(const StrView<CharT> &text) const -> bool
        {
            return terminals.matches(text).success;
        }

        CERBLIB_DECL auto isComment(const StrView<CharT> &text) const -> bool
        {
            return basicIsComment(text, comment_tokens.single_line) ||
                   basicIsComment(text, comment_tokens.multiline_begin) ||
                   basicIsComment(text, comment_tokens.multiline_end);
        }

    private:
        CERBLIB_DECL auto
            basicIsComment(const StrView<CharT> &text, const StrView<CharT> &comment) const -> bool
        {
            return not comment.empty() && text.substr(0, comment.size()) == comment;
        }

    public:
        CERBLIB_DECL auto isStringOrChar(const StrView<CharT> &text) const -> bool
        {
            for (auto &str : strings_and_chars) {
                auto substr = text.substr(0, str.str.size());

                if (substr == str.str) {
                    return true;
                }
            }

            return false;
        }

        CommentTokens comment_tokens{};
        std::vector<String<CharT>> strings_and_chars{};
        StringMap<CharT, size_t> terminals{};
    };
}// namespace cerb::lex

#endif /* CERBERUS_PROJECT_ANALYSIS_SHARED_HPP */
