#ifndef CCL_PROJECT_RAII_HPP
#define CCL_PROJECT_RAII_HPP

#include <type_traits>

namespace ccl
{
    template<typename Fn, typename... Args>
    concept Callable = std::is_invocable_v<Fn, Args...>;

    template<Callable Constructor, Callable Deleter>
    class Raii
    {
    private:
        Optional<Constructor> constructor;
        Optional<Deleter> deleter;

    public:
        constexpr Raii(Constructor &&t_constructor, Deleter &&t_deleter)
          : constructor{std::forward<Constructor>(t_constructor)}
          , deleter{std::forward<Deleter>(t_deleter)}
        {
            (*constructor)();
        }

        Raii(const Raii &) = delete;
        Raii(Raii &&) noexcept = default;

        constexpr ~Raii()
        {
            if (deleter.has_value()) {
                (*deleter)();
            }
        }

        auto operator=(const Raii &) -> Raii & = delete;
        auto operator=(Raii &&) noexcept -> Raii & = default;
    };
}// namespace ccl

#endif /* CCL_PROJECT_RAII_HPP */
