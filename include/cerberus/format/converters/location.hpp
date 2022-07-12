#ifndef CERBERUS_PROJECT_FMT_LOCATION_HPP
#define CERBERUS_PROJECT_FMT_LOCATION_HPP

#include <cerberus/format/converters/string.hpp>
#include <cerberus/template_string.hpp>
#include <cerberus/text/text_iterator_modules/location.hpp>

namespace cerb::fmt
{
    template<TemplateString String, typename... Args>
    CERBLIB_DECL auto format(Args &&...args) -> std::basic_string<decltype(String.zeroChar())>;

    template<CharacterLiteral To, TemplateString String, typename... Args>
    CERBLIB_DECL auto format(Args &&...args) -> std::basic_string<To>;

    template<CharacterLiteral CharT1, CharacterLiteral CharT2>
    constexpr auto convert(
        std::basic_string<CharT1> &formatting_string,
        const text::Location<CharT2> &location)
    {
        auto formatted_string = fmt::format<CharT1, "filename: {}, line: {}, column: {}">(
            location.getFilename(), location.getLine(), location.getColumn());

        convert(formatting_string, formatted_string);
    }
}// namespace cerb::fmt

#endif /* CERBERUS_PROJECT_FMT_LOCATION_HPP */
