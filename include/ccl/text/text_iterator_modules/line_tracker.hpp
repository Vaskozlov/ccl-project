#ifndef CCL_PROJECT_LINE_TRACKER_HPP
#define CCL_PROJECT_LINE_TRACKER_HPP

#include <ccl/ccl.hpp>
#include <isl/string_view.hpp>

namespace ccl::text
{
    class LineTracker
    {
    private:
        isl::string_view text;
        isl::string_view line;
        bool newLinePassed{};

    public:
        [[nodiscard]] explicit LineTracker(isl::string_view text_to_track) noexcept;

        [[nodiscard]] auto get() const noexcept CCL_LIFETIMEBOUND -> const isl::string_view &
        {
            return line;
        }

        auto next(char32_t chr) noexcept -> void;

    private:
        auto updateLine() -> void;
    };
}// namespace ccl::text

#endif /* CCL_PROJECT_LINE_TRACKER_HPP */
