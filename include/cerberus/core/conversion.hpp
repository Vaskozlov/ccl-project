#ifndef CERBERUS_PROJECT_CONVERSION_HPP
#define CERBERUS_PROJECT_CONVERSION_HPP

#include <array>
#include <cerberus/core/concepts.hpp>
#include <cerberus/core/exception.hpp>
#include <numeric>

namespace cerb
{
    template<CharacterLiteral To, CharacterLiteral From, size_t N>
    consteval auto strCast(const std::array<From, N> &str) -> std::array<To, N>
    {
        auto upper_limit = std::numeric_limits<To>::max();
        auto lower_limit = std::numeric_limits<To>::min();

        std::array<To, N> result{};

        for (size_t i = 0; i != N; ++i) {
            auto chr = str[i];

            if (chr >= lower_limit && chr <= upper_limit) {
                result[i] = static_cast<To>(chr);
            } else {
                throw LogicError("unable to convert string");
            }
        }

        return result;
    }

    template<CharacterLiteral To, CharacterLiteral From, size_t N>// NOLINTNEXTLINE
    consteval auto strCast(const From (&str)[N]) -> std::array<To, N>
    {
        return strCast<To>(std::to_array(str));
    }
}// namespace cerb

#endif /* CERBERUS_PROJECT_CONVERSION_HPP */
