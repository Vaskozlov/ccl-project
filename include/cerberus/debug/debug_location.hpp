#ifndef CERBERUS_PROJECT_DEBUG_LOCATION_HPP
#define CERBERUS_PROJECT_DEBUG_LOCATION_HPP

#include <cerberus/core/defines.hpp>
#include <fmt/format.h>
#include <string>
#include <string_view>

#if !defined(__PRETTY_FUNCTION__) && defined(_MSC_VER)
#    define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

#define CREATE_DEBUG_LOCATION cerb::debug::Location(__FILE__, __PRETTY_FUNCTION__, __LINE__)

namespace cerb::debug
{
    class Location
    {
    public:
        CERBLIB_DECL auto getFilename() const -> std::string_view const &
        {
            return file;
        }

        CERBLIB_DECL auto getFunction() const -> std::string_view const &
        {
            return function;
        }

        CERBLIB_DECL auto getLine() const -> size_t
        {
            return line;
        }

        [[nodiscard]] auto toStr() const -> std::string
        {
            return ::fmt::format(
                "file: {}, function: {}, line: {}", getFilename(), getFunction(), getLine());
        }

        Location() = default;

        constexpr Location(std::string_view file_, std::string_view function_, size_t line_)
          : file(file_), function(function_), line(line_)
        {}

    private:
        std::string_view file{};
        std::string_view function{};
        size_t line{};
    };
}// namespace cerb::debug

#endif /* CERBERUS_PROJECT_DEBUG_LOCATION_HPP */
