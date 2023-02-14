#ifndef CCL_PROJECT_LAZY_HPP
#define CCL_PROJECT_LAZY_HPP

#include <ccl/ccl.hpp>

namespace ccl
{
    template<typename T>
    concept LazyStorable = std::is_trivially_copyable_v<T> && std::is_trivially_destructible_v<T> &&
                           std::is_trivially_copy_assignable_v<T>;

    template<LazyStorable T>
    class Lazy
    {
    private:
        std::variant<std::function<T()>, T> lazyObject;

    public:
        Lazy() = default;

        // NOLINTNEXTLINE
        constexpr Lazy(T object)
          : lazyObject{std::forward<T>(object)}
        {}

        template<Callable Func>
        constexpr explicit Lazy(Func &&function)
          : lazyObject{std::forward<Func>(function)}
        {}

        // NOLINTNEXTLINE
        CCL_DECL operator T()
        {
            return get();
        }

        // NOLINTNEXTLINE
        CCL_DECL operator T() const
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
