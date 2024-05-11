#ifndef CCL_PROJECT_LOCATION_HPP
#define CCL_PROJECT_LOCATION_HPP

#include <ccl/char.hpp>
#include <isl/string_view.hpp>
#include <isl/utf8.hpp>

namespace ccl::text
{
    class Location
    {
        isl::string_view filename;
        std::size_t line{1};
        std::size_t column{0};
        std::size_t realColumn{0};

    public:
        Location() noexcept = default;

        [[nodiscard]] constexpr explicit Location(
            isl::string_view name_of_file, std::size_t line_in_file = 1, std::size_t column_in_file = 0,
            std::size_t real_column = 0) noexcept
          : filename{name_of_file}
          , line{line_in_file}
          , column{column_in_file}
          , realColumn{real_column}
        {}

        CCL_DECL auto getLine() const noexcept -> std::size_t
        {
            return line;
        }

        CCL_DECL auto getColumn() const noexcept -> std::size_t
        {
            return column;
        }

        CCL_DECL auto getRealColumn() const noexcept -> std::size_t
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
class fmt::formatter<ccl::text::Location>
{
public:
    [[maybe_unused]] constexpr static auto parse(const format_parse_context &ctx)
    {
        return ctx.begin();
    }

    template<typename FmtContext>
    constexpr auto format(const ccl::text::Location &location, FmtContext &ctx) const
    {
        return fmt::format_to(
            ctx.out(), "{}, line: {}, column: {}", location.getFilename(), location.getLine(),
            location.getColumn());
    }
};

#endif /* CCL_PROJECT_LOCATION_HPP */
