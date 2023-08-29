#ifndef CCL_PROJECT_RAII_HPP
#define CCL_PROJECT_RAII_HPP

#include <ccl/ccl.hpp>
#include <type_traits>

namespace ccl
{
    template<Invocable Deleter, Invocable Constructor = void(*)()>
    class Raii
    {
    private:
        Optional<Constructor> constructor;
        Optional<Deleter> deleter;

    public:
        [[nodiscard]] explicit constexpr Raii(Deleter &&t_deleter)
          : deleter{std::move(t_deleter)}
        {}

        [[nodiscard]] constexpr Raii(Constructor &&t_constructor, Deleter &&t_deleter)
          : constructor{std::move(t_constructor)}
          , deleter{std::move(t_deleter)}
        {
            (*constructor)();
        }

        [[nodiscard]] constexpr Raii(Raii &&other) noexcept
          : constructor{std::exchange(other.constructor, std::nullopt)}
          , deleter{std::exchange(other.deleter, std::nullopt)}
        {}

        Raii(const Raii &) = delete;

        constexpr ~Raii()
        {
            if (deleter.has_value()) {
                (*deleter)();
            }
        }

        auto operator=(const Raii &) -> Raii & = delete;

        constexpr auto operator=(Raii &&other) noexcept -> Raii &
        {
            std::swap(constructor, other.constructor);
            std::swap(deleter, other.deleter);
        }
    };
}// namespace ccl

#endif /* CCL_PROJECT_RAII_HPP */
