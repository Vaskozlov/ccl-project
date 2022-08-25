#ifndef CCL_PROJECT_FMT_LOCATION_HPP
#define CCL_PROJECT_FMT_LOCATION_HPP

#include <ccl/const_string.hpp>
#include <ccl/format/dumpers/string.hpp>
#include <ccl/text/location.hpp>

namespace ccl::fmt
{
    template<ConstString String, typename... Args>
    CCL_DECL auto format(Args &&...args) -> std::u8string;

    constexpr auto dump(std::u8string &formatting_string, const text::Location &location)
    {
        auto formatted_string = fmt::format<u8"{}, line: {}, column: {}">(
            location.getFilename(), location.getLine(), location.getColumn());

        dump(formatting_string, formatted_string);
    }
}// namespace ccl::fmt

#endif /* CCL_PROJECT_FMT_LOCATION_HPP */
