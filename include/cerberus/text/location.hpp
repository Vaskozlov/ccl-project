#ifndef CERBERUS_PROJECT_LOCATION_HPP
#define CERBERUS_PROJECT_LOCATION_HPP

#include <cerberus/cerberus.hpp>
#include <string_view>

namespace cerb::text
{
    template<CharacterLiteral CharT = char>
    class Location
    {
    public:
        CERBLIB_DECL auto getLine() const -> size_t
        {
            return line;
        }

        CERBLIB_DECL auto getColumn() const -> size_t
        {
            return column;
        }

        CERBLIB_DECL auto getOffset() const -> size_t
        {
            return offset;
        }

        CERBLIB_DECL auto getFilename() const -> const std::basic_string_view<CharT> &
        {
            return filename;
        }

        constexpr auto nextChar() -> void
        {
            ++column;
            ++offset;
        }

        constexpr auto nextLine() -> void
        {
            column = 1;
            ++line;
            ++offset;
        }

        Location() = default;

        constexpr explicit Location(
            std::basic_string_view<CharT> filename_, size_t line_ = 1, size_t column_ = 1,
            size_t offset_ = 0)
          : filename(filename_), line(line_), column(column_), offset(offset_)
        {}

    private:
        std::basic_string_view<CharT> filename{};
        size_t line{ 1 };
        size_t column{ 1 };
        size_t offset{ 0 };
    };
}// namespace cerb::text

#endif /* CERBERUS_PROJECT_LOCATION_HPP */
