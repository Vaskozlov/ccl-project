#ifndef CCL_PROJECT_LOCATION_HPP
#define CCL_PROJECT_LOCATION_HPP

#include <ccl/char.hpp>
#include <isl/string_view.hpp>

namespace ccl::text
{
    struct Location
    {
        u32 line{1};
        u32 column{};

        constexpr auto next(const char32_t chr) noexcept -> void
        {
            const auto is_new_line = chr == U'\n';
            column = is_new_line ? 0 : column + 1;
            line += static_cast<u32>(is_new_line);
        }
    };
}// namespace ccl::text

template<>
struct fmt::formatter<ccl::text::Location> : formatter<std::string_view>
{
    static auto
        format(const ccl::text::Location &location, format_context &ctx) -> format_context::iterator
    {
        return fmt::format_to(ctx.out(), "line: {}, column: {}", location.line, location.column);
    }
};

#endif /* CCL_PROJECT_LOCATION_HPP */
