#ifndef CCL_PROJECT_LINE_TRACKER_HPP
#define CCL_PROJECT_LINE_TRACKER_HPP

#include <ccl/string_view.hpp>

namespace ccl::text
{
    class LineTracker
    {
    private:
        string_view text{};
        string_view line{};
        bool newLinePassed{false};

    public:
        [[nodiscard]] explicit LineTracker(string_view text_to_track) noexcept;

        [[nodiscard]] auto get() const noexcept -> const string_view &
        {
            return line;
        }

        auto next(char32_t chr) noexcept -> void;

    private:
        auto updateLine() -> void;
    };
}// namespace ccl::text

#endif /* CCL_PROJECT_LINE_TRACKER_HPP */
