#include <ccl/lex/dot_item/logical_unit.hpp>

namespace ccl::lex::dot_item
{
    LogicalUnit::LogicalUnit(
        UniquePtr<BasicItem> lhs_, UniquePtr<BasicItem> rhs_, LogicalOperation type_, Id id_)
      : BasicItem{id_}
      , rhsItem{std::move(lhs_)}
      , lhsItem{std::move(rhs_)}
      , logicalOperation{type_}
    {}

    auto LogicalUnit::empty() const noexcept -> bool
    {
        return rhsItem->empty() && lhsItem->empty();
    }

    auto LogicalUnit::scanIteration(const ForkedGenerator &text_iterator) const -> size_t
    {
        switch (logicalOperation) {
        case LogicalOperation::OR:
            return orIteration(text_iterator);

        case LogicalOperation::AND:
            return andIteration(text_iterator);

        default:
            std::unreachable();
        }
    }

    auto LogicalUnit::orIteration(const ForkedGenerator &text_iterator) const -> size_t
    {
        return *rhsItem->scan(text_iterator)
                    .or_else([this, &text_iterator]() { return lhsItem->scan(text_iterator); })
                    .or_else([]() -> Optional<size_t> { return 0ZU; });
    }

    auto LogicalUnit::andIteration(const ForkedGenerator &text_iterator) const -> size_t
    {
        const auto lhs = rhsItem->scan(text_iterator);

        if (!lhs.has_value()) {
            return 0;
        }

        const auto rhs = lhsItem->scan(text_iterator);

        if (rhs.has_value() && (lhs == rhs)) {
            return *rhs;
        }

        return 0;
    }
}// namespace ccl::lex::dot_item
