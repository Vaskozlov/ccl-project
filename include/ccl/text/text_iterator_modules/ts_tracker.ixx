module;
#include <ccl/defines.hpp>
export module ccl.text.modules:ts_tracker;

export import isl;

export namespace ccl::text::modules
{
    class TsTracker
    {
    private:
        std::string tabsAnsSpaces{};
        bool needToClear{};

        static auto isTabOrSpace(char32_t chr) noexcept -> bool
        {
            return '\t' == chr || ' ' == chr;
        }

    public:
        [[nodiscard]] auto size() const noexcept -> std::size_t
        {
            return tabsAnsSpaces.size();
        }

        [[nodiscard]] auto empty() const noexcept -> bool
        {
            return tabsAnsSpaces.empty();
        }

        [[nodiscard]] auto get() const noexcept CCL_LIFETIMEBOUND -> const std::string &
        {
            return tabsAnsSpaces;
        }

        auto next(char32_t chr) -> void
        {
            clearIfNeed();

            if (isTabOrSpace(chr)) {
                tabsAnsSpaces.push_back(isl::as<char>(chr));
            } else {
                needToClear = true;
            }
        }

    private:
        auto clearIfNeed() noexcept -> void
        {
            if (needToClear) {
                needToClear = false;
                tabsAnsSpaces.clear();
            }
        }
    };
}// namespace ccl::text
