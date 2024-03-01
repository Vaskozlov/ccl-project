module;
#include <ccl/defines.hpp>
export module ccl.text.modules:ts_tracker;

export import std;

export namespace ccl::text::modules
{
    class TsTracker
    {
    private:
        std::string tabsAnsSpaces{};
        bool needToClear{};

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

        auto next(char32_t chr) -> void;

    private:
        auto clearIfNeed() noexcept -> void;
    };
}// namespace ccl::text
