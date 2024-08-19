#ifndef CCL_PROJECT_BINARY_OPERATION_AND_HPP
#define CCL_PROJECT_BINARY_OPERATION_AND_HPP

#include <ccl/lexer/rule/binary_expression/binary_expression_base.hpp>

namespace ccl::lexer::rule
{
    class BinaryOperationAnd final : public BinaryExpressionBase
    {
    public:
        using BinaryExpressionBase::BinaryExpressionBase;

        [[nodiscard]] auto
            scanIteration(const ForkedGenerator &text_iterator) const -> ScanResult override;
    };
}// namespace ccl::lexer::rule

#endif /* CCL_PROJECT_BINARY_OPERATION_AND_HPP */
