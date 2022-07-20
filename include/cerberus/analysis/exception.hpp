#ifndef CERBERUS_PROJECT_ANALYSIS_EXCEPTION_HPP
#define CERBERUS_PROJECT_ANALYSIS_EXCEPTION_HPP

#include <cerberus/cerberus.hpp>
#include <fmt/format.h>
#include <vector>

namespace cerb::analysis
{
    template<Exception T>
    class ExceptionAccumulator
    {
    public:
        CERBLIB_DECL auto getErrors() -> std::vector<std::unique_ptr<T>> &
        {
            return errors;
        }

        CERBLIB_DECL auto getErrors() const -> const std::vector<std::unique_ptr<T>> &
        {
            return errors;
        }

        CERBLIB_DECL auto getWarnings() -> std::vector<std::unique_ptr<T>> &
        {
            return warnings;
        }

        CERBLIB_DECL auto getWarnings() const -> const std::vector<std::unique_ptr<T>> &
        {
            return warnings;
        }

        constexpr auto clearErrors() -> void
        {
            errors.clear();
        }

        constexpr auto clearWarnings() -> void
        {
            warnings.clear();
        }

        template<Exception U>
        constexpr auto addError(const U &exception) -> void
        {
            static_assert(std::is_base_of_v<T, U>);
            errors.emplace_back(std::make_unique<U>(exception));
        }

        template<Exception U>
        constexpr auto addError(U &&exception) -> void
        {
            static_assert(std::is_base_of_v<T, U>);
            errors.emplace_back(std::make_unique<U>(exception));
        }

        template<Exception U>
        constexpr auto addWarning(const U &exception) -> void
        {
            static_assert(std::is_base_of_v<T, U>);
            warnings.emplace_back(std::make_unique<U>(exception));
        }

        template<Exception U>
        constexpr auto addWarning(U &&exception) -> void
        {
            static_assert(std::is_base_of_v<T, U>);
            warnings.emplace_back(std::make_unique<U>(exception));
        }

        ExceptionAccumulator() = default;

    private:
        std::vector<std::unique_ptr<T>> errors{};
        std::vector<std::unique_ptr<T>> warnings{};
    };

    template<Exception T, Exception BaseException>
    constexpr auto
        addError(ExceptionAccumulator<BaseException> *exception_accumulator, T &&exception) -> void
    {
        if (exception_accumulator == nullptr) {
            throw exception;
        }

        exception_accumulator->addError(std::forward<T>(exception));
    }

    template<Exception T, Exception BaseException>
    constexpr auto
        addError(ExceptionAccumulator<BaseException> *exception_accumulator, const T &exception)
            -> void
    {
        if (exception_accumulator == nullptr) {
            throw exception;
        }

        exception_accumulator->addError(exception);
    }

    template<Exception T, Exception BaseException>
    constexpr auto
        addWarning(ExceptionAccumulator<BaseException> *exception_accumulator, T &&exception)
            -> void
    {
        if (exception_accumulator == nullptr) {
            ::fmt::print("{}\n", exception.what());
            return;
        }

        exception_accumulator->addWarning(std::forward<T>(exception));
    }

    template<Exception T, Exception BaseException>
    constexpr auto
        addWarning(ExceptionAccumulator<BaseException> *exception_accumulator, const T &exception)
            -> void
    {
        if (exception_accumulator == nullptr) {
            ::fmt::print("{}\n", exception.what());
            return;
        }

        exception_accumulator->addWarning(exception);
    }
}// namespace cerb::analysis

#endif /* CERBERUS_PROJECT_ANALYSIS_EXCEPTION_HPP */
