#ifndef CERBERUS_PROJECT_LINE_TRACKER_HPP
#define CERBERUS_PROJECT_LINE_TRACKER_HPP

#include <cerberus/string_view.hpp>
#include <string>

namespace cerb::text::module
{
    class LineTracker
    {
    public:
        CERBLIB_DECL auto get() const noexcept -> const u8string_view &
        {
            return line;
        }

        constexpr auto next(char32_t chr) noexcept -> void
        {
            if (new_line_passed) {
                updateLine();
                new_line_passed = false;
            }

            if (chr == U'\n') {
                new_line_passed = true;
            }
        }

        constexpr explicit LineTracker(const u8string_view &text_) noexcept
          : text(text_), line(text.begin(), std::min(text.size(), text.find<UNSAFE>('\n')))
        {}

    private:
        constexpr auto updateLine() -> void
        {
            const auto *new_line_begin = std::min(text.end(), line.end() + 1);
            auto end_offset = std::min(text.size(), text.find<UNSAFE>(U'\n', new_line_begin));

            line = { new_line_begin, text.begin() + end_offset };
        }

        u8string_view text{};
        u8string_view line{};
        bool new_line_passed{ false };
    };
}// namespace cerb::text::module

#endif /* CERBERUS_PROJECT_LINE_TRACKER_HPP */
