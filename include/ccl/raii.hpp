#ifndef CCL_PROJECT_RAII_HPP
#define CCL_PROJECT_RAII_HPP

#include <ccl/ccl.hpp>
#include <type_traits>

namespace ccl
{
    template<Invocable DeleterFunction, Invocable ConstructorFunction = void(*)()>
    class Raii
    {
    private:
        std::optional<ConstructorFunction> constructorFunction;
        std::optional<DeleterFunction> deleterFunction;

    public:
        [[nodiscard]] explicit constexpr Raii(DeleterFunction &&t_deleter)
          : deleterFunction{std::move(t_deleter)}
        {}

        [[nodiscard]] constexpr Raii(
            ConstructorFunction &&t_constructor, DeleterFunction &&t_deleter)
          : constructorFunction{std::move(t_constructor)}
          , deleterFunction{std::move(t_deleter)}
        {
            (*constructorFunction)();
        }

        [[nodiscard]] constexpr Raii(Raii &&other) noexcept
          : constructorFunction{std::exchange(other.constructorFunction, std::nullopt)}
          , deleterFunction{std::exchange(other.deleterFunction, std::nullopt)}
        {}

        Raii(const Raii &) = delete;

        constexpr ~Raii()
        {
            if (deleterFunction.has_value()) {
                (*deleterFunction)();
            }
        }

        auto operator=(const Raii &) -> Raii & = delete;

        constexpr auto operator=(Raii &&other) noexcept -> Raii &
        {
            std::swap(constructorFunction, other.constructorFunction);
            std::swap(deleterFunction, other.deleterFunction);
        }
    };
}// namespace ccl

#endif /* CCL_PROJECT_RAII_HPP */
