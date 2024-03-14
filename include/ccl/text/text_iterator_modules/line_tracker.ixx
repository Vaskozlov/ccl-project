module;

#include <ccl/defines.hpp>

export module ccl.text.modules:line_tracker;

export import isl;

export namespace ccl::text::modules
{
    class LineTracker
    {
    private:
        isl::string_view text{};
        isl::string_view line{};
        bool newLinePassed{false};

    public:
        [[nodiscard]] explicit LineTracker(isl::string_view text_to_track) noexcept
          : text{text_to_track}
        {
            const auto new_line_index = text.find('\n');
            const auto line_end_index = new_line_index.value_or(text.size());

            line = {text.begin(), line_end_index};
        }

        [[nodiscard]] auto get() const noexcept CCL_LIFETIMEBOUND -> const isl::string_view &
        {
            return line;
        }

        auto next(char32_t chr) noexcept -> void
        {
            if (newLinePassed) [[unlikely]] {
                updateLine();
            }

            newLinePassed = '\n' == chr;
        }

    private:
        auto updateLine() -> void
        {
            const auto *new_line_begin = std::min(text.end(), line.end() + 1);
            const auto new_line_index = text.find('\n', new_line_begin);
            const auto *line_end = text.begin() + new_line_index.value_or(text.size());

            line = {new_line_begin, std::min(text.end(), line_end)};
        }
    };
}// namespace ccl::text::modules
