#ifndef CERBERUS_PROJECT_LEX_TYPEDEFS_HPP
#define CERBERUS_PROJECT_LEX_TYPEDEFS_HPP

#include <cerberus/text/typedefs.hpp>

namespace cerb::lex
{
    template<CharacterLiteral CharT>
    using Str = text::Str<CharT>;

    template<CharacterLiteral CharT>
    using StrView = text::StrView<CharT>;
}// namespace cerb::lex

#endif /* CERBERUS_PROJECT_LEX_TYPEDEFS_HPP */
