#ifndef CCL_PROJECT_BINARY_OPERATION_AND_HPP
#define CCL_PROJECT_BINARY_OPERATION_AND_HPP

#include <ccl/lexer/dot_item/binary_expression/binary_expression_base.hpp>

namespace ccl::lexer::dot_item
{
    class BinaryOperationAnd final : public BinaryExpressionBase
    {
    public:
        using BinaryExpressionBase::BinaryExpressionBase;

        [[nodiscard]] auto scanIteration(const ForkedGenerator &text_iterator) const
            -> ScanResult override;

        [[nodiscard]] auto
            parseIteration(const ForkedGenerator &text_iterator) const -> ParsingResult override;
    };
}// namespace ccl::lexer::dot_item

#endif /* CCL_PROJECT_BINARY_OPERATION_AND_HPP */
