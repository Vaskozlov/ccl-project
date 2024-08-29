#ifndef CCL_PROJECT_LOCATION_HPP
#define CCL_PROJECT_LOCATION_HPP

#include <ccl/char.hpp>
#include <isl/string_view.hpp>

namespace ccl::text
{
    class Location
    {
        isl::string_view filename;
        u32 line{1};
        u32 column{};
        u32 realColumn{};

    public:
        Location() noexcept = default;

        [[nodiscard]] constexpr explicit Location(
            isl::string_view name_of_file, u32 line_in_file = 1, u32 column_in_file = 0,
            u32 real_column = 0) noexcept
          : filename{name_of_file}
          , line{line_in_file}
          , column{column_in_file}
          , realColumn{real_column}
        {}

        CCL_DECL auto getLine() const noexcept -> u32
        {
            return line;
        }

        CCL_DECL auto getColumn() const noexcept -> u32
        {
            return column;
        }

        CCL_DECL auto getRealColumn() const noexcept -> u32
        {
            return realColumn;
        }

        CCL_DECL auto getFilename() const noexcept -> isl::string_view
        {
            return filename;
        }

        constexpr auto intermediateNext(char chr) noexcept -> void
        {
            if (!isEoF(chr) && chr != '\n') {
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
struct fmt::formatter<ccl::text::Location>
{
    [[maybe_unused]] constexpr static auto
        parse(const format_parse_context &ctx) -> format_parse_context::iterator
    {
        return ctx.begin();
    }

    static auto
        format(const ccl::text::Location &location, format_context &ctx) -> format_context::iterator
    {
        return fmt::format_to(
            ctx.out(), "{}, line: {}, column: {}", location.getFilename(), location.getLine(),
            location.getColumn());
    }
};

#endif /* CCL_PROJECT_LOCATION_HPP */
