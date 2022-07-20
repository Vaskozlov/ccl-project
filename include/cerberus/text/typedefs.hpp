#ifndef CERBERUS_PROJECT_TYPEDEFS_HPP
#define CERBERUS_PROJECT_TYPEDEFS_HPP

#include <cerberus/string_view.hpp>

namespace cerb::text
{
    template<CharacterLiteral CharT>
    using Str = std::basic_string<CharT>;

    template<CharacterLiteral CharT>
    using StrView = BasicStringView<CharT>;
}// namespace cerb::text

#endif /* CERBERUS_PROJECT_TYPEDEFS_HPP*/
