#ifndef CERBERUS_PROJECT_TS_TRACKER_HPP
#define CERBERUS_PROJECT_TS_TRACKER_HPP

#include <cerberus/text/typedefs.hpp>
#include <string>

namespace cerb::text::module
{
    template<CharacterLiteral CharT>
    class TsTracker
    {
    public:
        CERBLIB_DECL auto size() const -> size_t
        {
            return tabs_and_spaces.size();
        }

        CERBLIB_DECL auto empty() const -> bool
        {
            return tabs_and_spaces.empty();
        }

        CERBLIB_DECL auto get() const -> const Str<CharT> &
        {
            return tabs_and_spaces;
        }

        constexpr auto next(CharT chr) -> void
        {
            clearIfNeeded();

            if (isTs(chr)) {
                tabs_and_spaces.push_back(chr);
            } else {
                need_to_clear = true;
            }
        }

        TsTracker() = default;

    private:
        constexpr auto clearIfNeeded() -> void
        {
            if (need_to_clear) {
                tabs_and_spaces.clear();
                need_to_clear = false;
            }
        }

        CERBLIB_DECL static auto isTs(CharT chr) -> bool
        {
            return lor(chr == '\t', chr == ' ');
        }

        Str<CharT> tabs_and_spaces{};
        bool need_to_clear{};
    };
}// namespace cerb::text::module

#endif /* CERBERUS_PROJECT_TS_TRACKER_HPP */
