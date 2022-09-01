#ifndef CCL_PROJECT_LOCATION_HPP
#define CCL_PROJECT_LOCATION_HPP

#include <ccl/char.hpp>
#include <ccl/string_view.hpp>
#include <ccl/utf8.hpp>

namespace ccl::text
{
    class Location
    {
    public:
        Location() noexcept = default;

        constexpr explicit Location(
            string_view filename_, size_t line_ = 1, size_t column_ = 0,
            size_t real_column_ = 0) noexcept
          : filename(filename_), line(line_), column(column_), real_column(real_column_)
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
            return real_column;
        }

        CCL_DECL auto getFilename() const noexcept -> const string_view &
        {
            return filename;
        }

        constexpr auto intermediateNext(char chr) noexcept -> void
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

    private:
        string_view filename{};
        size_t line{ 1 };
        size_t column{ 0 };
        size_t real_column{ 0 };
    };
}// namespace ccl::text

template<>
class fmt::formatter<ccl::text::Location>
{
public:
    constexpr static auto parse(format_parse_context &ctx)
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
