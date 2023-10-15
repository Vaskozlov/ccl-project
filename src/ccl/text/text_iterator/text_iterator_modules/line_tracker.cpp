#include "ccl/text/text_iterator_modules/line_tracker.hpp"

namespace ccl::text
{
    LineTracker::LineTracker(isl::string_view text_to_track) noexcept
      : text{text_to_track}
    {
        const auto new_line_index = text.find('\n');
        const auto line_end_index = new_line_index.value_or(text.size());

        line = {text.begin(), line_end_index};
    }

    auto LineTracker::next(char32_t chr) noexcept -> void
    {
        if (newLinePassed) [[unlikely]] {
            updateLine();
        }

        newLinePassed = '\n' == chr;
    }

    auto LineTracker::updateLine() -> void
    {
        const auto *new_line_begin = std::min(text.end(), line.end() + 1);
        const auto new_line_index = text.find('\n', new_line_begin);
        const auto *line_end = text.begin() + new_line_index.value_or(text.size());

        line = {new_line_begin, std::min(text.end(), line_end)};
    }
}// namespace ccl::text