#ifndef CCL_PROJECT_ANALYSIS_EXCEPTION_HPP
#define CCL_PROJECT_ANALYSIS_EXCEPTION_HPP

#include <ccl/ccl.hpp>
#include <fmt/format.h>
#include <vector>

namespace ccl::analysis
{
    CCL_EXCEPTION(ExceptionAccumulatorOverflow, CclException);

    template<Exception T>
    class ExceptionAccumulator
    {
    public:
        CCL_DECL auto getErrors() noexcept -> std::vector<std::unique_ptr<T>> &
        {
            return errors;
        }

        CCL_DECL auto getErrors() const noexcept -> const std::vector<std::unique_ptr<T>> &
        {
            return errors;
        }

        CCL_DECL auto getWarnings() noexcept -> std::vector<std::unique_ptr<T>> &
        {
            return warnings;
        }

        CCL_DECL auto getWarnings() const noexcept -> const std::vector<std::unique_ptr<T>> &
        {
            return warnings;
        }

        constexpr auto clearErrors() noexcept -> void
        {
            errors.clear();
        }

        constexpr auto clearWarnings() noexcept -> void
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
}// namespace ccl::analysis

#endif /* CCL_PROJECT_ANALYSIS_EXCEPTION_HPP */
