#ifndef CERBERUS_PROJECT_GENERATOR_HPP
#define CERBERUS_PROJECT_GENERATOR_HPP

#include <coroutine>
#include <exception>

namespace cerb::coro
{
    template<typename T>
    struct [[nodiscard]] Generator
    {
        struct promise_type;
        using coro_handle = std::coroutine_handle<promise_type>;

        explicit Generator(coro_handle handle_) noexcept : handle{ handle_ }
        {}

        Generator(const Generator &) = delete;

        Generator(Generator &&other) noexcept : handle{ other.handle }
        {
            other.handle = nullptr;
        }

        ~Generator()
        {
            if (handle) {
                handle.destroy();
            }
        }

        auto operator=(const Generator &) = delete;

        auto operator=(Generator &&other) noexcept -> Generator &
        {
            std::swap(handle, other.handle);
            return *this;
        }

        auto resume() -> bool
        {
            if (not handle.done()) {
                handle.resume();
            }

            return not handle.done();
        }

        auto next() -> bool
        {
            if (handle) {
                handle.resume();
            }

            return not handle.done();
        }

        auto getValue() -> T &
        {
            return handle.promise().value;
        }

        auto operator()() -> T &
        {
            next();
            return getValue();
        }

    private:
        coro_handle handle;
    };

    template<typename T>
    struct Generator<T>::promise_type
    {
        using coro_handle = typename Generator<T>::coro_handle;

        auto get_return_object() -> Generator<T>
        {
            return Generator<T>{ coro_handle::from_promise(*this) };
        }

        auto initial_suspend() noexcept -> std::suspend_always
        {
            return {};
        }

        auto final_suspend() noexcept -> std::suspend_always
        {
            return {};
        }

        auto return_void() -> void
        {}

        auto unhandled_exception() -> void
        {
            std::terminate();
        }

        auto yield_value(const T &new_value) -> std::suspend_always
        {
            value = new_value;
            return std::suspend_always{};
        }

        T value;
    };
}// namespace cerb::coro

#endif /* CERBERUS_PROJECT_GENERATOR_HPP */
