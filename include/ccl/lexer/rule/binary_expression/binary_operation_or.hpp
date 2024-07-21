#ifndef CCL_PROJECT_BINARY_OPERATION_OR_HPP
#define CCL_PROJECT_BINARY_OPERATION_OR_HPP

#include "binary_expression_base.hpp"

namespace ccl::lexer::rule
{
    class BinaryOperationOr final : public BinaryExpressionBase
    {
    public:
        using BinaryExpressionBase::BinaryExpressionBase;

        [[nodiscard]] auto
            scanIteration(const ForkedGenerator &text_iterator) const -> ScanResult override;

        [[nodiscard]] auto
            parseIteration(const ForkedGenerator &text_iterator) const -> ParsingResult override;

        [[nodiscard]] auto clone() const -> isl::UniquePtr<RuleBlockInterface> override
        {
            return isl::makeUnique<BinaryOperationOr>(
                RuleBlock{getLhs()->clone()}, RuleBlock{getRhs()->clone()}, getId());
        }
    };
}// namespace ccl::lexer::rule

#endif /* CCL_PROJECT_BINARY_OPERATION_OR_HPP */
