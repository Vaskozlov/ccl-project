#ifndef CCL_PROJECT_RAII_HPP
#define CCL_PROJECT_RAII_HPP

namespace ccl
{
    template<typename T, typename... Ts>
    concept Callable =
        requires(T &&function, Ts &&...args) { function(std::forward<Ts>(args)...); };

    template<Callable Constructor, Callable Deleter>
    class Raii
    {
    private:
        Constructor constructor;
        Deleter deleter;

    public:
        Raii(Constructor &&t_constructor, Deleter &&t_deleter)
          : constructor{std::forward<Constructor>(t_constructor)}
          , deleter{std::forward<Deleter>(t_deleter)}
        {
            constructor();
        }

        Raii(const Raii &) = delete;
        Raii(Raii &&) noexcept = default;

        ~Raii()
        {
            deleter();
        }

        auto operator=(const Raii &) = delete;
        auto operator=(Raii &&) noexcept = default;
    };
}// namespace ccl

#endif /* CCL_PROJECT_RAII_HPP */
