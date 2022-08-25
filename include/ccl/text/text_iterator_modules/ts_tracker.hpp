#ifndef CCL_PROJECT_TS_TRACKER_HPP
#define CCL_PROJECT_TS_TRACKER_HPP

#include <ccl/ccl.hpp>
#include <string>

namespace ccl::text::module
{
    class TsTracker
    {
    public:
        CCL_DECL auto size() const noexcept -> size_t
        {
            return tabs_and_spaces.size();
        }

        CCL_DECL auto empty() const noexcept -> bool
        {
            return tabs_and_spaces.empty();
        }

        CCL_DECL auto get() const noexcept -> const std::u32string &
        {
            return tabs_and_spaces;
        }

        constexpr auto next(char32_t chr) -> void
        {
            clearIfNeed();

            if (isTabOrSpace(chr)) {
                tabs_and_spaces.push_back(chr);
            } else {
                need_to_clear = true;
            }
        }

        TsTracker() = default;

    private:
        constexpr auto clearIfNeed() noexcept -> void
        {
            if (need_to_clear) {
                need_to_clear = false;
                tabs_and_spaces.clear();
            }
        }

        CCL_DECL static auto isTabOrSpace(char32_t chr) noexcept -> bool
        {
            return lor(chr == '\t', chr == ' ');
        }

        std::u32string tabs_and_spaces{};
        bool need_to_clear{};
    };
}// namespace ccl::text::module

#endif /* CCL_PROJECT_TS_TRACKER_HPP */
