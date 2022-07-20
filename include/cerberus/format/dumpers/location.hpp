#ifndef CERBERUS_PROJECT_FMT_LOCATION_HPP
#define CERBERUS_PROJECT_FMT_LOCATION_HPP

#include <cerberus/const_string.hpp>
#include <cerberus/format/dumpers/string.hpp>
#include <cerberus/text/location.hpp>

namespace cerb::fmt
{
    template<ConstString String, typename... Args>
    CERBLIB_DECL auto format(Args &&...args) -> std::basic_string<decltype(String.zeroChar())>;

    template<CharacterLiteral To, ConstString String, typename... Args>
    CERBLIB_DECL auto format(Args &&...args) -> std::basic_string<To>;

    template<CharacterLiteral CharT1, CharacterLiteral CharT2>
    constexpr auto
        dump(std::basic_string<CharT1> &formatting_string, const text::Location<CharT2> &location)
    {
        auto formatted_string = fmt::format<CharT1, "{}, line: {}, column: {}">(
            location.getFilename(), location.getLine(), location.getColumn());

        dump(formatting_string, formatted_string);
    }
}// namespace cerb::fmt

#endif /* CERBERUS_PROJECT_FMT_LOCATION_HPP */
