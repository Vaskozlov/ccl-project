#ifndef CCL_PROJECT_LAZY_HPP
#define CCL_PROJECT_LAZY_HPP

#include <ccl/ccl.hpp>
#include <functional>
#include <variant>

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
        [[nodiscard]] constexpr explicit Lazy(T object)
          : lazyObject{std::move(object)}
        {}

        template<Invocable Func>
        [[nodiscard]] constexpr explicit Lazy(Func &&function)
          : lazyObject{std::forward<Func>(function)}
        {}

        CCL_DECL auto get() -> T &
        {
            compute();
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
            if (lazyObject.index() == 0) {
                lazyObject = std::get<0>(lazyObject)();
            }
        }
    };

    template<typename T>
    CCL_DECL auto toLazy(T &&value) -> Lazy<std::remove_cvref_t<T>>
        requires(LazyStorable<std::remove_cvref_t<T>>)
    {
        return Lazy{std::forward<T>(value)};
    }

    template<Invocable Func>
    CCL_DECL auto toLazy(Func &&function) -> Lazy<decltype(function())>
        requires(LazyStorable<decltype(function())>)
    {
        return Lazy<decltype(function())>(std::forward<Func>(function));
    }
}// namespace ccl

#endif /* CCL_PROJECT_LAZY_HPP */
