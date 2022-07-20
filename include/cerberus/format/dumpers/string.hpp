#ifndef CERBERUS_PROJECT_STRING_HPP
#define CERBERUS_PROJECT_STRING_HPP

#include <cerberus/string_view.hpp>
#include <string>
#include <string_view>

namespace cerb::fmt
{
    namespace private_
    {
        template<CharacterLiteral CharT1, CharacterLiteral CharT2>
        constexpr auto dumpDifferentStrings(
            std::basic_string<CharT2> &formatting_string,
            const std::basic_string_view<CharT1> &string) -> void
        {
            constexpr auto upper_limit = std::numeric_limits<CharT2>::max();
            constexpr auto lower_limit = std::numeric_limits<CharT2>::min();

            for (auto &chr : string) {
                if (chr >= lower_limit && chr <= upper_limit) {
                    formatting_string.push_back(static_cast<CharT2>(chr));
                } else {
                    formatting_string.push_back('?');
                }
            }
        }
    }// namespace private_

    template<CharacterLiteral CharT1, CharacterLiteral CharT2>
    constexpr auto dump(
        std::basic_string<CharT1> &formatting_string,
        const std::basic_string_view<CharT2> &string) -> void
    {
        if constexpr (std::is_same_v<CharT1, CharT2>) {
            formatting_string.append(string);
        } else {
            private_::dumpDifferentStrings(formatting_string, string);
        }
    }

    template<CharacterLiteral CharT1, CharacterLiteral CharT2>
    constexpr auto dump(std::basic_string<CharT1> &formatting_string, const CharT2 *string) -> void
    {
        if constexpr (std::is_same_v<CharT1, CharT2>) {
            formatting_string.append(string);
        } else {
            dump(formatting_string, std::basic_string_view<CharT2>{ string });
        }
    }

    template<CharacterLiteral CharT1, CharacterLiteral CharT2>
    constexpr auto
        dump(std::basic_string<CharT1> &formatting_string, const std::basic_string<CharT2> &string)
            -> void
    {
        if constexpr (std::is_same_v<CharT1, CharT2>) {
            formatting_string.append(string);
        } else {
            dump(formatting_string, std::basic_string_view{ string.c_str(), string.size() });
        }
    }

    template<CharacterLiteral CharT1, CharacterLiteral CharT2>
    constexpr auto
        dump(std::basic_string<CharT1> &formatting_string, const BasicStringView<CharT2> &string)
    {
        dump(formatting_string, static_cast<std::basic_string_view<CharT2>>(string));
    }
}// namespace cerb::fmt

#endif /* CERBERUS_PROJECT_STRING_HPP */
