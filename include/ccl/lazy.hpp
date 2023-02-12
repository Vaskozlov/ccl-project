#ifndef CCL_PROJECT_LAZY_HPP
#define CCL_PROJECT_LAZY_HPP

#include <ccl/core/defines.hpp>

namespace ccl
{
    template<typename T>
    class Lazy
    {
    private:
        std::variant<std::function<T()>, T> lazyObject;

    public:
        Lazy() = default;

        constexpr explicit Lazy(T &&object)
          : lazyObject{std::forward<T>(object)}
        {}

        template<typename Func>
        constexpr explicit Lazy(Func &&function)
          : lazyObject{std::forward<Func>(function)}
        {}

        // NOLINTNEXTLINE
        CCL_DECL operator T()
            requires(std::is_trivial_v<T>)
        {
            return get();
        }

        // NOLINTNEXTLINE
        CCL_DECL operator T() const
            requires(std::is_trivial_v<T>)
        {
            return get();
        }

        CCL_DECL auto get() -> T &
        {
            if (lazyObject.index() == 0) {
                compute();
            }

            return std::get<1>(lazyObject);
        }

        CCL_DECL auto get() const -> T
        {
            if (lazyObject.index() == 0) {
                return std::get<0>(lazyObject)();
            }

            return std::get<1>(lazyObject);
        }

    private:
        constexpr auto compute() -> void
        {
            lazyObject = std::get<0>(lazyObject)();
        }
    };
}// namespace ccl

#endif /* CCL_PROJECT_LAZY_HPP */
