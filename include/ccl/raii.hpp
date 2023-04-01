#ifndef CCL_PROJECT_RAII_HPP
#define CCL_PROJECT_RAII_HPP

#include <ccl/ccl.hpp>
#include <type_traits>

namespace ccl
{
    template<Invocable Constructor, Invocable Deleter>
    class Raii
    {
    private:
        Optional<Constructor> constructor;
        Optional<Deleter> deleter;

    public:
        constexpr Raii(Constructor &&t_constructor, Deleter &&t_deleter)
          : constructor{std::move(t_constructor)}
          , deleter{std::move(t_deleter)}
        {
            (*constructor)();
        }

        Raii(const Raii &) = delete;

        constexpr Raii(Raii &&other) noexcept
          : constructor{std::move(other.constructor)}
          , deleter{std::move(other.deleter)}
        {
            other.constructor = std::nullopt;
            other.deleter = std::nullopt;
        }

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
