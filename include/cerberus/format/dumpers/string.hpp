#ifndef CERBERUS_PROJECT_STRING_HPP
#define CERBERUS_PROJECT_STRING_HPP

#include <cerberus/const_string.hpp>
#include <cerberus/utf8.hpp>
#include <string>
#include <string_view>

namespace cerb::fmt
{
    constexpr auto dump(std::u8string &formatting_string, const std::u8string_view &string) -> void
    {
        if CERBLIB_COMPILE_TIME_BRANCH {
            for (auto &&chr : string) {
                formatting_string.push_back(chr);
            }
        } else {
            formatting_string.append(string);
        }
    }

    constexpr auto dump(std::u8string &formatting_string, const char8_t *string) -> void
    {
        formatting_string.append(string);
    }

    constexpr auto dump(std::u8string &formatting_string, const std::u8string &string) -> void
    {
        formatting_string.append(string);
    }

    constexpr auto dump(std::u8string &formatting_string, const u8string_view &string) -> void
    {
        dump(formatting_string, static_cast<std::u8string_view>(string));
    }

    template<size_t N>
    constexpr auto dump(std::u8string &formatting_string, const ConstString<N> &string) -> void
    {
        dump(formatting_string, u8string_view(string.begin(), string.end()));
    }
}// namespace cerb::fmt

#endif /* CERBERUS_PROJECT_STRING_HPP */
