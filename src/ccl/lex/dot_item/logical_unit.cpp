#include <ccl/lex/dot_item/logical_unit.hpp>

namespace ccl::lex::dot_item
{
    LogicalUnit::LogicalUnit(
        DotItem lhs, DotItem rhs, LogicalOperation logical_operation, Id item_id)
      : DotItemConcept{item_id}
      , rhsItem{std::move(rhs)}
      , lhsItem{std::move(lhs)}
      , logicalOperation{logical_operation}
    {}

    auto LogicalUnit::empty() const noexcept -> bool
    {
        return rhsItem->empty() && lhsItem->empty();
    }

    auto LogicalUnit::scanIteration(const ForkedGenerator &text_iterator) const -> size_t
    {
        switch (logicalOperation) {
        [[likely]] case LogicalOperation::OR:
            return orIteration(text_iterator);

        case LogicalOperation::AND:
            return andIteration(text_iterator);

        default:
            unreachable();
        }
    }

    auto LogicalUnit::orIteration(const ForkedGenerator &text_iterator) const -> size_t
    {
        const auto lhs_scan_result = lhsItem->scan(text_iterator);

        if (lhs_scan_result.has_value()) {
            return *lhs_scan_result;
        }

        return rhsItem->scan(text_iterator).value_or(0);
    }

    auto LogicalUnit::andIteration(const ForkedGenerator &text_iterator) const -> size_t
    {
        const auto lhs = rhsItem->scan(text_iterator);

        if (!lhs.has_value()) [[likely]] {
            return 0;
        }

        const auto rhs = lhsItem->scan(text_iterator);

        if (rhs.has_value() && (lhs == rhs)) {
            return *lhs;
        }

        return 0;
    }
}// namespace ccl::lex::dot_item
