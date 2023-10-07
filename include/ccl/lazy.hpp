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
    class Lazy : public std::variant<std::function<T()>, T>
    {
    public:
        using std::variant<std::function<T()>, T>::index;
        using std::variant<std::function<T()>, T>::variant;

        CCL_DECL auto get() const -> T
        {
            if (this->index() == 0) {
                return std::get<0>(*this)();
            }

            return std::get<1>(*this);
        }

        CCL_DECL auto get() CCL_LIFETIMEBOUND->T &
        {
            compute();
            return std::get<1>(*this);
        }

    private:
        constexpr auto compute() -> void
        {
            if (index() == 0) {
                *this = std::get<0>(*this)();
            }
        }
    };

    template<typename T>
    CCL_DECL auto toLazy(T &&value) -> Lazy<std::remove_cvref_t<T>>
        requires(LazyStorable<std::remove_cvref_t<T>>)
    {
        return Lazy<T>{std::forward<T>(value)};
    }

    template<Invocable Func>
    CCL_DECL auto toLazy(Func &&function) -> Lazy<decltype(function())>
        requires(LazyStorable<std::invoke_result_t<Func>>)
    {
        return Lazy<decltype(function())>(std::forward<Func>(function));
    }
}// namespace ccl

#endif /* CCL_PROJECT_LAZY_HPP */
