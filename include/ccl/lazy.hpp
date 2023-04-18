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
    class [[nodiscard]] Lazy
    {
    private:
        std::variant<std::function<T()>, T> lazyObject;

    public:
        Lazy() = default;

        // NOLINTNEXTLINE
        constexpr explicit Lazy(T object)
          : lazyObject{std::move(object)}
        {}

        template<Invocable Func>
        constexpr explicit Lazy(Func &&function)
          : lazyObject{std::forward<Func>(function)}
        {}

        constexpr auto get() -> T &
        {
            if (lazyObject.index() == 0) {
                compute();
            }

            return std::get<1>(lazyObject);
        }

        constexpr auto get() const -> T
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
