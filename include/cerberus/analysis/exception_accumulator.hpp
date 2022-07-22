#ifndef CERBERUS_PROJECT_ANALYSIS_EXCEPTION_HPP
#define CERBERUS_PROJECT_ANALYSIS_EXCEPTION_HPP

#include <cerberus/cerberus.hpp>
#include <fmt/format.h>
#include <vector>

namespace cerb::analysis
{
    CERBLIB_EXCEPTION(ExceptionAccumulatorOverflow, CerberusException);

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
        constexpr friend auto addError(ExceptionAccumulator *accumulator, U &&exception) -> void
        {
            basicAddError(accumulator, std::forward<U>(exception));
        }

        template<Exception U>
        constexpr friend auto addWarning(ExceptionAccumulator *accumulator, U &&exception) -> void
        {
            basicAddWarning(accumulator, std::forward<U>(exception));
        }

        ExceptionAccumulator() = default;

        constexpr explicit ExceptionAccumulator(size_t max_errors_) : max_errors(max_errors_)
        {}

    private:
        template<Exception U>
        constexpr friend auto basicAddError(ExceptionAccumulator *accumulator, U &&exception)
            -> void
        {
            static_assert(std::is_base_of_v<T, U>);

            if (accumulator == nullptr) {
                throw exception;
            }

            accumulator->errors.emplace_back(std::make_unique<U>(std::forward<U>(exception)));
        }


        template<Exception U>
        constexpr friend auto basicAddWarning(ExceptionAccumulator *accumulator, U &&exception)
            -> void
        {
            static_assert(std::is_base_of_v<T, U>);

            if (accumulator == nullptr) {
                ::fmt::print("{}\n", exception.what());
                return;
            }

            accumulator->warnings.emplace_back(std::make_unique<U>(std::forward<U>(exception)));
        }

        std::vector<std::unique_ptr<T>> errors{};
        std::vector<std::unique_ptr<T>> warnings{};
        size_t max_errors{ 100 };// NOLINT
    };
}// namespace cerb::analysis

#endif /* CERBERUS_PROJECT_ANALYSIS_EXCEPTION_HPP */
