#ifndef CCL_PROJECT_LOCATION_HPP
#define CCL_PROJECT_LOCATION_HPP

#include <ccl/char.hpp>
#include <ccl/string_view.hpp>
#include <ccl/utf8.hpp>

namespace ccl::text
{
    class Location
    {
        string_view filename{};
        size_t line{1};
        size_t column{0};
        size_t realColumn{0};

    public:
        Location() noexcept = default;

        constexpr explicit Location(
            string_view name_of_file, size_t line_in_file = 1, size_t column_in_file = 0,
            size_t real_column = 0) noexcept
          : filename{name_of_file}
          , line{line_in_file}
          , column{column_in_file}
          , realColumn{real_column}
        {}

        CCL_DECL auto getLine() const noexcept -> size_t
        {
            return line;
        }

        CCL_DECL auto getColumn() const noexcept -> size_t
        {
            return column;
        }

        CCL_DECL auto getRealColumn() const noexcept -> size_t
        {
            return realColumn;
        }

        CCL_DECL auto getFilename() const noexcept -> const string_view &
        {
            return filename;
        }

        constexpr auto intermediateNext(char chr) noexcept -> void
        {
            if (land(!isEoF(chr), '\n' != chr)) {
                ++realColumn;
            }
        }
        constexpr auto next(char32_t chr) noexcept -> void
        {
            if (chr == U'\n') {
                ++line;
                column = realColumn = 0;
            } else if (!isEoF(chr)) {
                ++column;
            }
        }
    };
}// namespace ccl::text

template<>
class fmt::formatter<ccl::text::Location>
{
public:
    [[maybe_unused]] constexpr static auto parse(format_parse_context &ctx)
    {
        return ctx.begin();
    }

    template<typename FmtContext>
    constexpr auto format(const ccl::text::Location &location, FmtContext &ctx) const
    {
        return format_to(
            ctx.out(), "{}, line: {}, column: {}", location.getFilename(), location.getLine(),
            location.getColumn());
    }
};

#endif /* CCL_PROJECT_LOCATION_HPP */
