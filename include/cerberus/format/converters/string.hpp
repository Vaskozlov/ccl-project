#ifndef CERBERUS_PROJECT_STRING_HPP
#define CERBERUS_PROJECT_STRING_HPP

#include <string>
#include <string_view>

namespace cerb::fmt
{
    namespace private_
    {
        template<CharacterLiteral StrT, CharacterLiteral CharT>
        constexpr auto convertDifferentStrings(
            std::basic_string<CharT> &formatting_string, const std::basic_string_view<StrT> &string)
            -> void
        {
            constexpr auto upper_limit = std::numeric_limits<CharT>::max();
            constexpr auto lower_limit = std::numeric_limits<CharT>::min();

            for (auto &chr : string) {
                if (chr >= lower_limit && chr <= upper_limit) {
                    formatting_string.push_back(static_cast<CharT>(chr));
                } else {
                    formatting_string.push_back('?');
                }
            }
        }
    }// namespace private_

    template<CharacterLiteral StrT, CharacterLiteral CharT>
    constexpr auto convert(std::basic_string<CharT> &formatting_string, const StrT *string) -> void
    {
        if constexpr (std::is_same_v<CharT, StrT>) {
            formatting_string.append(string);
        } else {
            convert(formatting_string, std::basic_string_view<StrT>{ string });
        }
    }

    template<CharacterLiteral StrT, CharacterLiteral CharT>
    constexpr auto
        convert(std::basic_string<CharT> &formatting_string, const std::basic_string<StrT> &string)
            -> void
    {
        if constexpr (std::is_same_v<CharT, StrT>) {
            formatting_string.append(string);
        } else {
            convert(
                formatting_string, std::basic_string_view<StrT>{ string.c_str(), string.size() });
        }
    }

    template<CharacterLiteral StrT, CharacterLiteral CharT>
    constexpr auto convert(
        std::basic_string<CharT> &formatting_string, const std::basic_string_view<StrT> &string)
        -> void
    {
        if constexpr (std::is_same_v<CharT, StrT>) {
            formatting_string.append(string);
        } else {
            private_::convertDifferentStrings(formatting_string, string);
        }
    }
}// namespace cerb::fmt

#endif /* CERBERUS_PROJECT_STRING_HPP */
