#ifndef CERBERUS_PROJECT_TS_TRACKER_HPP
#define CERBERUS_PROJECT_TS_TRACKER_HPP

#include <cerberus/cerberus.hpp>
#include <string>

namespace cerb::text::module
{
    class TsTracker
    {
    public:
        CERBLIB_DECL auto size() const noexcept -> size_t
        {
            return tabs_and_spaces.size();
        }

        CERBLIB_DECL auto empty() const noexcept -> bool
        {
            return tabs_and_spaces.empty();
        }

        CERBLIB_DECL auto get() const noexcept -> const std::u32string &
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
                tabs_and_spaces.clear();
                need_to_clear = false;
            }
        }

        CERBLIB_DECL static auto isTabOrSpace(char32_t chr) noexcept -> bool
        {
            return lor(chr == '\t', chr == ' ');
        }

        std::u32string tabs_and_spaces{};
        bool need_to_clear{};
    };
}// namespace cerb::text::module

#endif /* CERBERUS_PROJECT_TS_TRACKER_HPP */
