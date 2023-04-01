#ifndef CCL_PROJECT_TS_TRACKER_HPP
#define CCL_PROJECT_TS_TRACKER_HPP

#include <ccl/ccl.hpp>

namespace ccl::text
{
    class TsTracker
    {
    private:
        std::string tabsAnsSpaces{};
        bool needToClear{};

    public:
        [[nodiscard]] auto size() const noexcept -> size_t
        {
            return tabsAnsSpaces.size();
        }

        [[nodiscard]] auto empty() const noexcept -> bool
        {
            return tabsAnsSpaces.empty();
        }

        [[nodiscard]] auto get() const noexcept -> const std::string &
        {
            return tabsAnsSpaces;
        }

        constexpr auto next(char32_t chr) -> void
        {
            clearIfNeed();

            if (isTabOrSpace(chr)) {
                tabsAnsSpaces.push_back(as<char>(chr));
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
            return lor('\t' == chr, ' ' == chr);
        }
    };
}// namespace ccl::text

#endif /* CCL_PROJECT_TS_TRACKER_HPP */
