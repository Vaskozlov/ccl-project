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
        CERBLIB_DECL auto getLine() const noexcept -> size_t
        {
            return line;
        }

        CERBLIB_DECL auto getColumn() const noexcept -> size_t
        {
            return column;
        }

        CERBLIB_DECL auto getRealColumn() const noexcept -> size_t
        {
            return real_column;
        }

        CERBLIB_DECL auto getFilename() const noexcept -> const u8string_view &
        {
            return filename;
        }

        constexpr auto intermediateNext(char8_t chr) noexcept -> void
        {
            if (land(not isEoF(chr), chr != '\n')) {
                ++real_column;
            }
        }
        constexpr auto next(char32_t chr) noexcept -> void
        {
            if (chr == U'\n') {
                ++line;
                column = 0;
                real_column = 0;
            } else if (not isEoF(chr)) {
                ++column;
            }
        }

        Location() noexcept = default;

        constexpr explicit Location(
            u8string_view filename_, size_t line_ = 1, size_t column_ = 0,
            size_t real_column_ = 0) noexcept
          : filename(filename_), line(line_), column(column_), real_column(real_column_)
        {}

    private:
        u8string_view filename{};
        size_t line{ 1 };
        size_t column{ 0 };
        size_t real_column{ 0 };
    };

}// namespace cerb::text

#endif /* CERBERUS_PROJECT_LOCATION_HPP */
