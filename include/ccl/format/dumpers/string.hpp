#ifndef CCL_PROJECT_STRING_HPP
#define CCL_PROJECT_STRING_HPP

#include <ccl/const_string.hpp>
#include <ccl/utf8.hpp>
#include <string>
#include <string_view>

namespace ccl::fmt
{
    constexpr auto dump(std::u8string &formatting_string, const std::u8string_view &string) -> void
    {
        if CCL_COMPILE_TIME_BRANCH {
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
        dump(formatting_string, static_cast<u8string_view>(string));
    }
}// namespace ccl::fmt

#endif /* CCL_PROJECT_STRING_HPP */
