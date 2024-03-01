module;
#include <ccl/defines.hpp>
export module ccl.lexer.dot_item.binary_expression:logical_or;

export import ccl.text;
export import :base_class;

export namespace ccl::lexer::dot_item
{
    class BinaryOperationOr final : public BinaryExpressionBase
    {
    public:
        using BinaryExpressionBase::BinaryExpressionBase;

        [[nodiscard]] auto scanIteration(const ForkedGenerator &text_iterator) const
            -> std::optional<std::size_t> override
        {
            const auto lhs_scan_result = lhsItem->scan(text_iterator);

            if (lhs_scan_result.has_value()) {
                return *lhs_scan_result;
            }

            return rhsItem->scan(text_iterator);
        }
    };
}// namespace ccl::lexer::dot_item
