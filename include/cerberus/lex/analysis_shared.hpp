#ifndef CERBERUS_PROJECT_ANALYSIS_SHARED_HPP
#define CERBERUS_PROJECT_ANALYSIS_SHARED_HPP

#include <cerberus/lex/typedefs.hpp>
#include <cerberus/string_map.hpp>
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
        std::vector<String<CharT>> strings_and_chars{};
        StringMap<CharT, size_t> terminals{};
    };
}// namespace cerb::lex

#endif /* CERBERUS_PROJECT_ANALYSIS_SHARED_HPP */
