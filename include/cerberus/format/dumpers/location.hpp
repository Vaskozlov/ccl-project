#ifndef CERBERUS_PROJECT_FMT_LOCATION_HPP
#define CERBERUS_PROJECT_FMT_LOCATION_HPP

#include <cerberus/const_string.hpp>
#include <cerberus/format/dumpers/string.hpp>
#include <cerberus/text/location.hpp>

namespace cerb::fmt
{
    template<ConstString String, typename... Args>
    CERBLIB_DECL auto format(Args &&...args) -> std::u8string;

    constexpr auto dump(std::u8string &formatting_string, const text::Location &location)
    {
        auto formatted_string = fmt::format<u8"{}, line: {}, column: {}">(
            location.getFilename(), location.getLine(), location.getColumn());

        dump(formatting_string, formatted_string);
    }
}// namespace cerb::fmt

#endif /* CERBERUS_PROJECT_FMT_LOCATION_HPP */
