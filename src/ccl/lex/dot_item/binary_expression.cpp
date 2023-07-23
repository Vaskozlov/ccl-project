#include <ccl/lex/dot_item/binary_expression.hpp>

namespace ccl::lex::dot_item
{
    BinaryExpression::BinaryExpression(
        DotItem lhs, DotItem rhs, BinaryOperator binary_operator, Id item_id)
      : DotItemConcept{item_id}
      , rhsItem{std::move(rhs)}
      , lhsItem{std::move(lhs)}
      , binaryOperator{binary_operator}
    {}

    auto BinaryExpression::empty() const noexcept -> bool
    {
        return rhsItem->empty() && lhsItem->empty();
    }

    auto BinaryExpression::scanIteration(const ForkedGenerator &text_iterator) const
        -> Optional<size_t>
    {
        switch (binaryOperator) {
        [[likely]] case BinaryOperator::OR:
            return orIteration(text_iterator);

        case BinaryOperator::AND:
            return andIteration(text_iterator);

        default:
            unreachable();
        }
    }

    auto BinaryExpression::orIteration(const ForkedGenerator &text_iterator) const
        -> Optional<size_t>
    {
        const auto lhs_scan_result = lhsItem->scan(text_iterator);

        if (lhs_scan_result.has_value()) {
            return *lhs_scan_result;
        }

        return rhsItem->scan(text_iterator);
    }

    auto BinaryExpression::andIteration(const ForkedGenerator &text_iterator) const
        -> Optional<size_t>
    {
        const auto lhs = rhsItem->scan(text_iterator);

        if (!lhs.has_value()) [[likely]] {
            return std::nullopt;
        }

        const auto rhs = lhsItem->scan(text_iterator);

        if (rhs.has_value() && (lhs == rhs)) {
            return *lhs;
        }

        return std::nullopt;
    }
}// namespace ccl::lex::dot_item
