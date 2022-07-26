#ifndef CERBERUS_PROJECT_LEX_TYPEDEFS_HPP
#define CERBERUS_PROJECT_LEX_TYPEDEFS_HPP

#include <cerberus/text/typedefs.hpp>

// NOLINTBEGIN
#define CERBLIB_DERIVED_CONSTRUCTORS(Class)                                                        \
    Class() = default;                                                                             \
    Class(const Class &) = default;                                                                \
    Class(Class &&) noexcept = default;                                                            \
    ~Class() = default;                                                                            \
    auto operator=(const Class &)->Class & = default;                                              \
    auto operator=(Class &&) noexcept->Class & = default
// NOLINTEND

namespace cerb::lex
{
    template<CharacterLiteral CharT>
    using Str = text::Str<CharT>;

    template<CharacterLiteral CharT>
    using StrView = text::StrView<CharT>;
}// namespace cerb::lex

#endif /* CERBERUS_PROJECT_LEX_TYPEDEFS_HPP */
