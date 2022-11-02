#ifndef CCL_PROJECT_TS_TRACKER_HPP
#define CCL_PROJECT_TS_TRACKER_HPP

#include <ccl/ccl.hpp>

namespace ccl::text
{
    class TsTracker
    {
    public:
        CCL_DECL auto size() const noexcept -> size_t
        {
            return tabsAnsSpaces.size();
        }

        CCL_DECL auto empty() const noexcept -> bool
        {
            return tabsAnsSpaces.empty();
        }

        CCL_DECL auto get() const noexcept -> const std::string &
        {
            return tabsAnsSpaces;
        }

        constexpr auto next(char32_t chr) -> void
        {
            clearIfNeed();

            if (isTabOrSpace(chr)) {
                tabsAnsSpaces.push_back(static_cast<char>(chr));
            } else {
                needToClear = true;
            }
        }

        TsTracker() = default;

    private:
        constexpr auto clearIfNeed() noexcept -> void
        {
            if (needToClear) {
                needToClear = false;
                tabsAnsSpaces.clear();
            }
        }

        CCL_DECL static auto isTabOrSpace(char32_t chr) noexcept -> bool
        {
            return lor(chr == '\t', chr == ' ');
        }

        std::string tabsAnsSpaces{};
        bool needToClear{};
    };
}// namespace ccl::text

#endif /* CCL_PROJECT_TS_TRACKER_HPP */
