#ifndef CCL_PROJECT_SCAN_RESULT_HPP
#define CCL_PROJECT_SCAN_RESULT_HPP

#include <ccl/ccl.hpp>
#include <numeric>

namespace ccl::lexer
{
    class CCL_TRIVIAL_ABI ScanResult
    {
    private:
        std::size_t value{};

    public:
        [[nodiscard]] constexpr static auto failure() noexcept -> ScanResult
        {
            return ScanResult{std::numeric_limits<std::size_t>::max()};
        }

        CCL_DECL explicit ScanResult(std::size_t bytesPassed) noexcept
          : value{bytesPassed}
        {}

        CCL_DECL auto operator==(const ScanResult &) const noexcept -> bool = default;

        CCL_DECL auto getBytesCount() const noexcept -> std::size_t
        {
            // TODO: add check???
            return value;
        }

        CCL_DECL auto isSuccess() const noexcept -> bool
        {
            return value != failure().value;
        }

        CCL_DECL auto isFailure() const noexcept -> bool
        {
            return !isSuccess();
        }

        CCL_DECL auto orElse(auto &&function) const -> ScanResult
        {
            if (isFailure()) {
                return function();
            }

            return *this;
        }
    };
}// namespace ccl::lexer

#endif /* CCL_PROJECT_SCAN_RESULT_HPP */
