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
        [[nodiscard]] explicit LineTracker(isl::string_view text_to_track) noexcept;

        [[nodiscard]] auto get() const noexcept CCL_LIFETIMEBOUND -> const isl::string_view &
        {
            return line;
        }

        auto next(char32_t chr) noexcept -> void;

    private:
        auto updateLine() -> void;
    };
}// namespace ccl::text::module
