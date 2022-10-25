#ifndef CCL_PROJECT_LINE_TRACKER_HPP
#define CCL_PROJECT_LINE_TRACKER_HPP

#include <ccl/string_view.hpp>
#include <string>

namespace ccl::text
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
            if (newLinePassed) [[unlikely]] {
                updateLine();
                newLinePassed = false;
            }

            newLinePassed = chr == '\n';
        }

        constexpr explicit LineTracker(const string_view &text_) noexcept
          : text(text_), line(text.begin(), std::min(text.size(), text.find<UNSAFE>('\n')))
        {}

    private:
        constexpr auto updateLine() -> void
        {
            auto end_offset = text.size();
            const auto *new_line_begin = std::min(text.end(), line.end() + 1);
            const auto new_line_index = text.find('\n', new_line_begin);

            if (new_line_index.has_value()) {
                end_offset = *new_line_index;
            }

            line = { new_line_begin, text.begin() + end_offset };
        }

        string_view text{};
        string_view line{};
        bool newLinePassed{ false };
    };
}// namespace ccl::text

#endif /* CCL_PROJECT_LINE_TRACKER_HPP */
