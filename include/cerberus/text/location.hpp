#ifndef CERBERUS_PROJECT_LOCATION_HPP
#define CERBERUS_PROJECT_LOCATION_HPP

#include <cerberus/char.hpp>
#include <cerberus/string_view.hpp>
#include <cerberus/utf8.hpp>

namespace cerb::text
{
    class Location
    {
    public:
        CERBLIB_DECL auto getLine() const -> size_t;
        CERBLIB_DECL auto getColumn() const -> size_t;
        CERBLIB_DECL auto getRealColumn() const -> size_t;
        CERBLIB_DECL auto getFilename() const -> const u8string_view &;

        constexpr auto intermediateNext(char8_t chr) -> void;
        constexpr auto next(char32_t chr) -> void;

        Location() = default;
        constexpr explicit Location(
            u8string_view filename_, size_t line_ = 1, size_t column_ = 0, size_t real_column_ = 0);

    private:
        u8string_view filename{};
        size_t line{ 1 };
        size_t column{ 0 };
        size_t real_column{ 0 };
    };

    constexpr auto Location::getLine() const -> size_t
    {
        return line;
    }

    constexpr auto Location::getColumn() const -> size_t
    {
        return column;
    }

    constexpr auto Location::getRealColumn() const -> size_t
    {
        return real_column;
    }

    constexpr auto Location::getFilename() const -> const u8string_view &
    {
        return filename;
    }

    constexpr auto Location::intermediateNext(char8_t chr) -> void
    {
        if (land(not isEoF(chr), chr != '\n')) {
            ++real_column;
        }
    }

    constexpr auto Location::next(char32_t chr) -> void
    {
        if (chr == U'\n') {
            ++line;
            column = 0;
            real_column = 0;
        } else if (not isEoF(chr)) {
            ++column;
        }
    }

    constexpr Location::Location(
        u8string_view filename_, size_t line_, size_t column_, size_t real_column_)
      : filename(filename_), line(line_), column(column_), real_column(real_column_)
    {}

}// namespace cerb::text

#endif /* CERBERUS_PROJECT_LOCATION_HPP */
