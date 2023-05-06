#include "ccl/text/text_iterator_modules/ts_tracker.hpp"

namespace ccl::text
{
    CCL_INLINE static auto isTabOrSpace(char32_t chr) noexcept -> bool
    {
        return lor('\t' == chr, ' ' == chr);
    }

    auto TsTracker::next(char32_t chr) -> void
    {
        clearIfNeed();

        if (isTabOrSpace(chr)) {
            tabsAnsSpaces.push_back(as<char>(chr));
        } else {
            needToClear = true;
        }
    }

    auto TsTracker::clearIfNeed() noexcept -> void
    {
        if (needToClear) {
            needToClear = false;
            tabsAnsSpaces.clear();
        }
    }
}// namespace ccl::text
