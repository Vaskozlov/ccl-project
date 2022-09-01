#ifndef CCL_PROJECT_LINE_TRACKER_HPP
#define CCL_PROJECT_LINE_TRACKER_HPP

#include <ccl/string_view.hpp>
#include <string>

namespace ccl::text::module
{
    class LineTracker
    {
    public:
        CCL_DECL auto get() const noexcept -> const string_view &
        {
            return line;
        }

        constexpr auto next(char32_t chr) noexcept -> void
        {
            if (new_line_passed) {
                updateLine();
                new_line_passed = false;
            }

            new_line_passed = chr == U'\n';
        }

        constexpr explicit LineTracker(const string_view &text_) noexcept
          : text(text_), line(text.begin(), std::min(text.size(), text.find<UNSAFE>('\n')))
        {}

    private:
        constexpr auto updateLine() -> void
        {
            const auto *new_line_begin = std::min(text.end(), line.end() + 1);
            auto end_offset = std::min(text.size(), text.find<UNSAFE>(U'\n', new_line_begin));

            line = { new_line_begin, text.begin() + end_offset };
        }

        string_view text{};
        string_view line{};
        bool new_line_passed{ false };
    };
}// namespace ccl::text::module

#endif /* CCL_PROJECT_LINE_TRACKER_HPP */
